/**
 * @file io_system.h
 * @author Álvaro Goldar Dieste
 * @date Oct 2020
 *
 * @brief This utility allows reading an input file, character by character.
 *
 * @details
 *  This utility allows reading the desired input file, character by
 *  character, in order to provide every utility that the delta's lexical
 *  analyzer may require to do its job. As of now, only one source file is
 *  supported.
 *
 *  This system's design follows the "sentinel buffer" method explained in the
 *  book "Compilers: Principles, Techniques, and Tools" by Alfred Aho et al.
 */


#ifndef D_IO_SYSTEM
#define D_IO_SYSTEM 


#include <stddef.h>


/**
 * @brief Represents an I/O system.
 *
 * @details
 *  Opaque data type which represents an I/O system.
 */
struct d_io_system;


/**
 * @brief Initializes an I/O system.
 *
 * @details
 *  Initializes an I/O system while also setting its desired properties, given
 *  as parameters.
 *
 * @param[out] io_system Reference to the pointer where the address of the new
 *                       I/O system will be stored.
 * @param[in] buffer_size Size N of the two internal buffers that the I/O system
 *                        will use; it must be multiple of ... // TODO. The
 *                        usable space in each buffer will be N-1 bytes (=
                          characters).
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_io_system_initialize(
    struct d_io_system **io_system,
    const size_t buffer_size
);


/**
 * @brief The I/O system opens a given file.
 *
 * @details
 *  The provided I/O system opens the requested file, and prepares its
 *  contents so that the may be read by any data consumer.
 *
 * @param[in,out] io_system The I/O system.
 * @param[in] file_path Path (relative or absolute) to the desired file.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_io_system_open_file(
    struct d_io_system *io_system,
    const char *file_path
);


/**
 * @brief Returns the next character that can be found in the input file.
 *
 * @details
 *  The I/O system returns the next character that can be found in the file,
 *  while also moving forward the file position indicator.
 *
 * @param[in,out] io_system The I/O system.
 * @param[out] character Reference to which the next character will be copied.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_io_system_get_next_char(
    struct d_io_system *io_system,
    unsigned char *next_character
);


/**
 * @brief Takes back the last character that has been found in the input file.
 *
 * @details
 *  The I/O system takes back the last character that has been found in the
 *  input file, therefore returning it the following time that the next
 *  character in the file is requested.
 *
 *  Warning: this system supports returning up to N-1 characters at a time at
 *           most, which will be enough in order to parse regression.d;
 *           that is, returning N or more characters without retrieving any of
 *           the first ones via "get_next_char" is undefined behaviour.
 *
 * @param[in,out] io_system The I/O system.
 * @param[in] character The character which is being returned.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_io_system_return_char(
    struct d_io_system *io_system,
    char character
);


/**
 * @brief Destroys an I/O system.
 *
 * @details
 *  Destroys the specified I/O system, while also closing its currently opened
 *  file, if any.
 *
 * @param[out] io_system Reference to the pointer where the address of the I/O
 *                       system can be found.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_io_system_destroy(
    struct d_io_system **io_system
);


#endif //D_IO_SYSTEM
