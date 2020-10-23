/**
 * @file io_system.c
 * @author Álvaro Goldar Dieste
 * @date Oct 2020
 *
 * @brief Implementation of io_system.h
 * 
 * @details
 *  It is worth noting that the input file will be mapped to memory as this
 *  method provides an increased transparency when working, in comparison with
 *  other file access methods, such as using a FILE data type, while also
 *  achieving extraordinary performance.
 */

// TODO most functions in this file will break if the given struct is not
// fully initialized

#include "io/io_system.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Memory mapping
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>


/**
 * @brief Represents an I/O system.
 *
 * @details
 *  Opaque data type which represents an I/O system.
 */
struct io_system {

    /** Pointer to the A buffer required by the sentinel method. */
    uint8_t *buffer_a;
    /** Pointer to the A buffer required by the sentinel method. */
    uint8_t *buffer_b;

    /** Size in bytes of each buffer. */
    size_t buffer_size;

    /** Pointer to the beginning of the lexeme which is going to be
     * recognized. */
    uint8_t *lexeme_begin;
    /** Pointer which is moved forward while reading characters in order to
     *  recognize the current lexeme. */
    uint8_t *forward;

    /** Pointer to the address to which the input file's contents can be
     *  found, as the file will be memory-mapped. */
    void *input_file;
    /** Size in bytes of the input file. */
    size_t input_file_size;
};


int io_system_initialize(
    struct io_system **io_system,
    const size_t buffer_size
)
{
    if(io_system == NULL) {

        perror("ERROR::IO_SYSTEM::Reference to struct io_system is NULL");
        return -1;
    }

    if(buffer_size <= 0) {

        perror("ERROR::IO_SYSTEM::Requested buffer size is <= 0");
        return -1;
    }


    // The structure that represents the I/O system gets initialized, as well
    // as all of its members

    if((*io_system = malloc(sizeof(struct io_system))) == NULL) {

        perror("ERROR::IO_SYSTEM::Could not allocate a struct io_system");
        return -1;
    }

    // TODO size must be multiple of... 
    if(((*io_system)->buffer_a = malloc(buffer_size)) == NULL) {

        perror("ERROR::IO_SYSTEM::Could not allocate buffer A");
        return -1;
    }

    if(((*io_system)->buffer_b = malloc(buffer_size)) == NULL) {

        perror("ERROR::IO_SYSTEM::Could not allocate buffer B");
        return -1;
    }

    (*io_system)->buffer_size = buffer_size;

    // At first, both position pointers are set at the beginning of the buffer
    // A
    (*io_system)->lexeme_begin = (*io_system)->buffer_a;
    (*io_system)->forward = (*io_system)->buffer_a;

    // No file is opened yet
    (*io_system)->input_file = NULL;


    return 0;
}


int io_system_open_file(
    struct io_system *io_system,
    const char* file_path
)
{
    int input_file_fd = 0;
    struct stat input_file_stats;


    if(io_system == NULL) {

        perror("ERROR::IO_SYSTEM::Reference to struct io_system is NULL");
        return -1;
    }
    
    if(file_path == NULL) {

        perror("ERROR::IO_SYSTEM::Reference to file path is NULL");
        return -1;
    }


    // As explained at the beginning of this file, the input file will be
    // memory mapped in order to ease the access to it
    
    // In order to do so, it must be first opened as a file descriptor
    if ((input_file_fd = open(file_path, O_RDONLY)) == -1) {

        perror("ERROR::IO_SYSTEM::Could not open input file as a file "
               "descriptor");
        return -1;
    }

    // Its size is also needed beforehand
    if(stat(file_path, &input_file_stats) == -1) {

        perror("ERROR::IO_SYSTEM::Could not retrieve input file size");
        return -1;
    }

    // Now, the input file can be finally mapped
    if((io_system->input_file = mmap(NULL, (size_t) input_file_stats.st_size,
                                     PROT_READ, MAP_SHARED, input_file_fd, 0))
        == MAP_FAILED) {

        perror("ERROR::IO_SYSTEM::Could not memory-map input file");
        return -1;
    }

    // Before exiting this function, the operating system is also pointed that
    // the data access pattern on the file will be mainly sequential
    if(madvise(io_system->input_file, (size_t) input_file_stats.st_size,
                MADV_SEQUENTIAL) == -1) {

        perror("ERROR::IO_SYSTEM::Could not advise about input file usage");
        return -1;
    }

    io_system->input_file_size = (size_t) input_file_stats.st_size;


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
int io_system_move_forward(
    struct io_system *io_system
)
{
    // TODO implement
}


unsigned char io_system_get_next_char(
    struct io_system *io_system,
    char *next_character
)
{
    if(io_system == NULL) {

        perror("ERROR::IO_SYSTEM::Reference to struct io_system is NULL");
        return -1;
    }
    
    if(next_character == NULL) {

        perror("ERROR::IO_SYSTEM::Reference to next character is NULL");
        return -1;
    }


    // The "forward" pointer will already be right on top of the next
    // character
    *next_character = (unsigned char) *(io_system->forward);

    // And the pointer will always automatically move ahead with each
    // character read
    io_system_move_forward(io_system);
}


int io_system_go_backwards(
    struct io_system *io_system
)
{
    // TODO implement
}


int io_system_destroy(
    struct io_system **io_system
)
{
    if(io_system == NULL) {

        perror("ERROR::IO_SYSTEM::Reference to struct io_system is NULL");
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
        
        perror("ERROR::IO_SYSTEM::Could not unmap input file");
        return -1;
    }

    // Pointers and other static members do not need to be cleared as the will
    // get destroyed in no time

    // The I/O structure itself
    free(*io_system);


    return 0;
}