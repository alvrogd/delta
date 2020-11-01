/**
 * @file io_system.c
 * @author Álvaro Goldar Dieste
 * @date Oct 2020
 *
 * @brief Implementation of io/io_system.h
 * 
 * @details
 *  It is worth noting that the input file will be mapped to memory as this
 *  method provides an increased transparency when working, in comparison with
 *  other file access methods, such as using a FILE data type, while also
 *  achieving extraordinary performance.
 */


/** Which character represents the EOF, according to the sentinel method. */
#define IO_SYSTEM_SENTINEL_EOF '\0'

/** Block size in bytes. */
// TODO As of now, the block size has been set to 4096 bytes as the compiler
//      is intended to be used in Linux, in which the most popular filesystem
//      is ext4, whose default block size is 4096 bytes. However, it would be
//      nice to automatically detect the block size of whichever filesystem
//      lies under.
#define IO_SYSTEM_BLOCK_SIZE 4096


#include "io/io_system.h"
#include "common/errors.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Memory mapping
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

// memcpy
#include <string.h>

// Block size
#include <linux/fs.h>


/**
 * @brief Represents an I/O system.
 *
 * @details
 *  Data type which represents an I/O system.
 */
struct d_io_system {

    /** Pointer to the A buffer required by the sentinel method. */
    uint8_t *buffer_a;
    /** Pointer to the address at which the A buffer ends. */
    uint8_t *buffer_a_end;
    /** Pointer to the A buffer required by the sentinel method. */
    uint8_t *buffer_b;
    /** Pointer to the address at which the A buffer ends. */
    uint8_t *buffer_b_end;

    /** Size in bytes of each buffer. */
    size_t buffer_size;

    /** Boolean that tells if the I/O system has been forced to be moved to
    the previous buffer due to returning a character. */ 
    uint8_t gone_backwards_buffer;

    /** Pointer to the beginning of the lexeme which is going to be
     * recognized. */
    uint8_t *lexeme_begin;
    /** Pointer which is moved forward while reading characters in order to
     *  recognize the current lexeme. */
    uint8_t *forward;
    /** How many characters the forward pointer has moved without recognizing
        a new lexeme. */
    size_t lexeme_length;

    /** Pointer to the address to which the input file's contents can be
     *  found, as the file will be memory-mapped. */
    void *input_file;
    /** Pointer to the file position in which the next characters that will
        be retrieved can be found. */
    void *input_file_position;
    /** Pointer to the address at which the input file's contents end. */
    void *input_file_end;

    /** Size in bytes of the input file. */
    size_t input_file_size;

    /** Bool that tells if the end of the input file has been reached. */
    int eof_reached;
};


/**
 * @brief Implementation of io_system.h/d_io_system_initialize
 */
int d_io_system_initialize(
    struct d_io_system **io_system,
    const size_t buffer_size
)
{
    if(io_system == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "io_system.c", "d_io_system_initialize",
                               "'io_system'");
        return -1;
    }

    if(buffer_size <= 0) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_INVALID,
                               "io_system.c", "d_io_system_initialize",
                               "'buffer_size' must be > 0");
        return -1;
    }


    // The structure that represents the I/O system gets initialized, as well
    // as all of its members

    if((*io_system = malloc(sizeof(struct d_io_system))) == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_SYSCALL_FAILED, "io_system.c",
                               "d_io_system_initialize",
                               "'malloc' for struct d_io_system");
        return -1;
    }

    // The buffer size is forced to be a multiple of the underlying block size
    //
    // If the specified size is not a multiple, the inmediate multiple that
    // is bigger than the requested size is chosen 
    (*io_system)->buffer_size = ((buffer_size + IO_SYSTEM_BLOCK_SIZE - 1) /
                                IO_SYSTEM_BLOCK_SIZE) * IO_SYSTEM_BLOCK_SIZE;
    // WARNING: this does not equal "(buffer_size + IO_SYSTEM_BLOCK_SIZE - 1)"
    //          due to the integer division
    #ifdef D_DEBUG
    printf("[io_system][initialize] Block size: %zu\n",
           (*io_system)->buffer_size);
    #endif

    if(((*io_system)->buffer_a = malloc((*io_system)->buffer_size)) == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_SYSCALL_FAILED, "io_system.c",
                               "d_io_system_initialize",
                               "'malloc' for buffer_a");
        return -1;
    }

    (*io_system)->buffer_a_end = (*io_system)->buffer_a +
                                 (*io_system)->buffer_size;

    if(((*io_system)->buffer_b = malloc((*io_system)->buffer_size)) == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_SYSCALL_FAILED, "io_system.c",
                               "d_io_system_initialize",
                               "'malloc' for buffer_b");
        return -1;
    }

    (*io_system)->buffer_b_end = (*io_system)->buffer_b +
                                 (*io_system)->buffer_size;


    // At first, both position pointers are set at the beginning of the buffer
    // A
    (*io_system)->lexeme_begin = (*io_system)->buffer_a;
    (*io_system)->forward = (*io_system)->buffer_a;
    (*io_system)->lexeme_length = 0;
    (*io_system)->gone_backwards_buffer = 0;

    // No file is opened yet
    (*io_system)->input_file = NULL;
    (*io_system)->input_file_position = NULL;
    (*io_system)->input_file_end = NULL;
    (*io_system)->input_file_size = 0;
    (*io_system)->eof_reached = 0;


    return 0;
}


/**
 * @brief Reads the next character data from the input file, storing it in the
 *        given buffer.
 *
 * @details
 *  Due to the "sentinel buffer" method, there will be two buffers which will
 *  be used in a round-robin fashion. In order to do so, once new data from
 *  the input file is needed, this function takes care of retrieving it and
 *  putting it in the desired buffer. The first N-1 bytes will be contents
 *  from the file, whereas the last byte is used as the EOF sentinel, may it
 *  mark the end of a buffer or the end of the input file.
 *
 *  It is worth noting that, as this system allows returning characters, there
 *  could be times that the desired data is already present in the next
 *  buffer, and so it will not need to be loaded.
 *
 * @param[in,out] io_system The I/O system.
 * @param[out] Pointer to the buffer.
 *
 * @return 0 if successful, any other value otherwise.
 */
int _d_io_system_fill_buffer(
    struct d_io_system *io_system,
    uint8_t *buffer
)
{
    size_t size_to_read = 0;


    #ifdef D_DEBUG
    printf("[io_system][fill buffer] Next buffer already loaded: %d\n",
           io_system->gone_backwards_buffer);
    #endif

    // If the I/O system had to move to the previous buffer due to returning
    // characters, the next buffer has already the expected file contents
    if(io_system->gone_backwards_buffer) {
        io_system->gone_backwards_buffer = 0;
    }

    // Otherwise, the contents must be loaded
    else {

        // Caution: there could not be enough remaining charactes in the input
        //          file to fill an entire buffer!
        //
        // As a side note, at most N-1 bytes will be read to be able to fit
        // the EOF character.
        size_to_read = io_system->input_file_end -
                       io_system->input_file_position;
        
        if((io_system->buffer_size - 1) < size_to_read) {
            size_to_read = io_system->buffer_size - 1;
        }

        memcpy(buffer, io_system->input_file_position, size_to_read);

        io_system->input_file_position += size_to_read;

        // In the position that comes after the last copied character...
        buffer[size_to_read] = IO_SYSTEM_SENTINEL_EOF;
    }

    #ifdef D_DEBUG
    printf("[io_system][fill buffer] Loaded contents: ");

    while(*buffer != IO_SYSTEM_SENTINEL_EOF){
        printf("%c, ", *buffer++);
    }

    printf("\n");
    #endif


    return 0;
}


/**
 * @brief Implementation of io_system.h/d_io_system_open_file
 */
int d_io_system_open_file(
    struct d_io_system *io_system,
    const char *file_path
)
{
    int input_file_fd = 0;
    struct stat input_file_stats;


    if(io_system == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "io_system.c", "d_io_system_open_file",
                               "'io_system'");
        return -1;
    }
    
    if(file_path == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "io_system.c", "d_io_system_open_file",
                               "'file_path'");
        return -1;
    }


    // As explained at the beginning of this file, the input file will be
    // memory mapped in order to ease the access to it
    
    // In order to do so, it must be first opened as a file descriptor
    if ((input_file_fd = open(file_path, O_RDONLY)) == -1) {

        d_errors_internal_show(3, D_ERR_USER_INPUT_FILE_INACCESSIBLE,
                               "io_system.c",
                               "d_io_system_open_file");
        return -1;
    }

    // Its size is also needed beforehand
    if(stat(file_path, &input_file_stats) == -1) {

        d_errors_internal_show(4, D_ERR_INTERN_SYSCALL_FAILED, "io_system.c",
                               "d_io_system_open_file",
                               "'stat' on input file");
        return -1;
    }

    // Now, the input file can be finally mapped
    if((io_system->input_file = mmap(NULL, (size_t) input_file_stats.st_size,
                                     PROT_READ, MAP_SHARED, input_file_fd, 0))
        == MAP_FAILED) {

        d_errors_internal_show(4, D_ERR_INTERN_SYSCALL_FAILED, "io_system.c",
                               "d_io_system_open_file",
                               "'mmap' on input file");
        return -1;
    }

    // Before concluding the mapping process, the operating system is also
    // pointed that the data access pattern on the file will be mainly
    // sequential
    if(madvise(io_system->input_file, (size_t) input_file_stats.st_size,
                MADV_SEQUENTIAL) == -1) {

        d_errors_internal_show(4, D_ERR_INTERN_SYSCALL_FAILED, "io_system.c",
                               "d_io_system_open_file",
                               "'madvise' on input file");
        return -1;
    }

    io_system->input_file_size = (size_t) input_file_stats.st_size;

    io_system->input_file_position = io_system->input_file;
    io_system->input_file_end = io_system->input_file +
                                io_system->input_file_size;

    // And the A buffer is the first to be filled with data from the file
    _d_io_system_fill_buffer(io_system, io_system->buffer_a);

    return 0;
}


/**
 * @brief The I/O system goes forward, so that it returns the next character
 *        in the file when requesting a new one.
 *
 * @details
 *  The I/O system moves forward its file position indicator, therefore
 *  setting it so that the next requested character is the one that goes after
 *  the last read character.
 *
 * @param[in,out] io_system The I/O system.
 *
 * @return 0 if successful, any other value otherwise.
 */
int _d_io_system_move_forward(
    struct d_io_system *io_system
)
{
    ++(io_system->forward);
    ++(io_system->lexeme_length);

    // If an EOF is found at the next position, it could mean that:
    //  - You have reached the end of the buffer A.
    //  - You have reached the end of the buffer B.
    //  - You have reached the end of the input file.
    if((unsigned char) *(io_system->forward) == IO_SYSTEM_SENTINEL_EOF) {

        // End of input file
        //
        // In order to have reached the end of the input file, the "current
        // position" pointer must be right where the "end" position was set.
        //
        // Furthermore, if the "I have moved backwards attribute" is active,
        // while the previous condition is met, it will mean that the next
        // buffer already contains any remaining contents from the input file,
        // but they have not been parsed yet as the I/O system moved backwards
        // from it, to the current buffer. Therefore, the system must not
        // report EOF as the currently read sentinel character would belong to
        // penultimate buffer, instead of the last one.
        if(io_system->input_file_position == io_system->input_file_end &&
           !io_system->gone_backwards_buffer) {
            
            io_system->eof_reached = 1;
        }

        else {

            // End of buffer A -> load data in buffer B if not present yet
            if(io_system->forward + 1 == io_system->buffer_a_end) {

                _d_io_system_fill_buffer(io_system, io_system->buffer_b);
                io_system->forward = io_system->buffer_b;
            }

            // End of buffer B -> load data in buffer A if not present yet
            else {
                _d_io_system_fill_buffer(io_system, io_system->buffer_a);
                io_system->forward = io_system->buffer_a;
            }
        }
    }

    // Otherwise, no action is required


    return 0;
}


/**
 * @brief Implementation of io_system.h/d_io_system_get_next_char
 */
int d_io_system_get_next_char(
    struct d_io_system *io_system,
    unsigned char *next_character
)
{
    if(io_system == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "io_system.c", "d_io_system_get_next_char",
                               "'io_system'");
        return -1;
    }
    
    if(next_character == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "io_system.c", "d_io_system_get_next_char",
                               "'next_character'");
        return -1;
    }

    // The "forward" pointer will already be right on top of the next
    // character
    *next_character = (unsigned char) *(io_system->forward);

    // And the pointer will always automatically move ahead with each
    // character read
    _d_io_system_move_forward(io_system);


    return 0;
}


/**
 * @brief Implementation of io_system.h/d_io_system_return_char
 */
int d_io_system_return_char(
    struct d_io_system *io_system,
    char character
)
{
    if(io_system == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "io_system.c", "d_io_system_return_char",
                               "'io_system'");
        return -1;
    }


    --(io_system->forward);
    --(io_system->lexeme_length);
    // Going backwards will always mean that eof has not been reached yet
    io_system->eof_reached = 0;

    // The problem with going backwards is that the I/O system may need to go
    // to the previous buffer if it is right at the beginning of the current
    // one

    // Gone past buffer A -> go back to buffer B
    if(io_system->forward + 1 == io_system->buffer_a) {

        io_system->gone_backwards_buffer = 1;
        // Going to the last element that is not EOF
        io_system->forward = io_system->buffer_b_end - 2;
    }

    else if (io_system->forward + 1 == io_system->buffer_b) {

        io_system->gone_backwards_buffer = 1;
        // Going to the last element that is not EOF
        io_system->forward = io_system->buffer_a_end - 2;
    }


    // Otherwise, no action is required


    return 0;    
}


/**
 * @brief Implementation of io_system.h/d_io_system_is_eof
 */
int d_io_system_is_eof(
    struct d_io_system *io_system
)
{
    if(io_system == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "io_system.c", "d_io_system_is_eof",
                               "'io_system'");
        return -1;
    }


    return io_system->eof_reached;
}


/**
 * @brief Implementation of io_system.h/d_io_system_save_current_lexeme
 */
int d_io_system_current_lexeme_recognized(
    struct d_io_system *io_system
)
{
    if(io_system == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "io_system.c",
                               "d_io_system_current_lexeme_recognized",
                               "'io_system'");
        return -1;
    }


    // The "lexeme start" pointer can now move to were the "forward" pointer
    // is in order to mark the start of the next lexeme
    io_system->lexeme_begin = io_system->forward;

    io_system->lexeme_length = 0;

    return 0;
}


/**
 * @brief Implementation of io_system.h/d_io_system_save_current_lexeme
 */
const unsigned char *d_io_system_save_current_lexeme(
    struct d_io_system *io_system
)
{
    unsigned char *lexeme = NULL;
    uint8_t *tmp_pointer = NULL;

    int i = 0;


    if(io_system == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "io_system.c",
                               "d_io_system_save_current_lexeme",
                               "'io_system'");
        return NULL;
    }


    // +1 in order to fit '\0' at the end
    if((lexeme = malloc((io_system->lexeme_length + 1) *
        sizeof(unsigned char))) == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_SYSCALL_FAILED, "io_system.c",
                               "d_io_system_save_current_lexeme",
                               "'malloc' for lexeme");
        return NULL;
    }

    lexeme[io_system->lexeme_length] = '\0';
    
    #ifdef D_DEBUG
    printf("[io_system][save lexeme] Lexeme length: %d\n", (int)
           io_system->lexeme_length);
    #endif


    // Each character between the "lexeme start" and "forward" pointers will
    // be copied one by one
    tmp_pointer = io_system->lexeme_begin;

    #ifdef D_DEBUG
    printf("[io_system][save lexeme] First char: %d\tFirst char address: "
           "%p\tBuffer A address: %p\tBuffer B address:%p\n", *tmp_pointer,
           tmp_pointer, io_system->buffer_a, io_system->buffer_b);
    #endif

    while(tmp_pointer != io_system->forward) {

        // "lexeme start" and "forward" pointers may not be in the same buffer
        if((unsigned char) *tmp_pointer == IO_SYSTEM_SENTINEL_EOF) {

            // End of buffer A -> tmp_pointer jumps to start of buffer B
            if(tmp_pointer + 1 == io_system->buffer_a_end) {

                tmp_pointer = io_system->buffer_b;
            }

            // End of buffer B -> tmp_pointer jumps to start of buffer A
            else {
                tmp_pointer = io_system->buffer_a;
            }
        }

        else {
            lexeme[i++] = *(tmp_pointer++);
        }
    }


    return lexeme;
}


/**
 * @brief Implementation of io_system.h/d_io_system_destroy
 */
int d_io_system_destroy(
    struct d_io_system **io_system
)
{
    if(io_system == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "io_system.c", "d_io_system_destroy",
                               "'io_system'");
        return -1;
    }


    // Every dynamically allocated member of the structure which represents
    // the I/O system gets destroyed, as well as the structure itself at the
    // end

    // Previous mallocs
    free((*io_system)->buffer_a);
    free((*io_system)->buffer_b);

    // The memory-mapped file
    if (munmap((*io_system)->input_file, (*io_system)->input_file_size)
        != 0) {
        
        d_errors_internal_show(4, D_ERR_INTERN_SYSCALL_FAILED, "io_system.c",
                               "d_io_system_destroy",
                               "'munmap' on input file");
        return -1;
    }

    // Pointers and other static members do not need to be cleared as the will
    // get destroyed in no time

    // The I/O structure itself
    free(*io_system);


    return 0;
}
