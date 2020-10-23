/**
 * @file io_system.h
 * @author Álvaro Goldar Dieste
 * @date Oct 2020
 *
 * @brief This utility allows reading an input file, character by character.
 *
 * @details
 *  This utility allows reading a desired input file, character by character,
 *  therefore providing every utility that the lexical analyzer may require.
 *
 *  Its design follows the "sentinel buffer" method explained in the book
 *  "Compilers: Principles, Techniques, and Tools" by Alfred Aho et al.
 */


#include <stddef.h>


/**
 * @brief Represents an I/O system.
 *
 * @details
 *  Opaque data type which represents an I/O system.
 */
struct io_system;


/**
 * @brief Initializes an I/O system.
 *
 * @details
 *  Initializes an I/O system while also setting its desired properties, given
 *  as parameters.
 *
 * @param[out] io_system Reference to the pointer where the address of the new
 *                       I/O system will be stored.
 * @param[in] buffer_size Size of the two internal buffers that the I/O system
 *                        will use; it must be a multiple of ...// TODO
 *
 * @return 0 if successful, any other value otherwise.
 */
int io_system_initialize(
    struct io_system **io_system,
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
int io_system_open_file(
    struct io_system *io_system,
    const char* file_path
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
 * @return The next character that can be found.
 */
unsigned char io_system_get_next_char(
    struct io_system *io_system,
    char *next_character
);


/**
 * @brief The I/O system goes backwards, so that it returns once again the
 *        last read character when requesting a new one.
 *
 * @details
 *  The I/O system moves backwards its file position indicator, therefore
 *  setting it so that the next requested character is once again the last
 *  read character.
 *
 * @param[in,out] io_system The I/O system.
 *
 * @return 0 if successful, any other value otherwise.
 */
int io_system_go_backwards(
    struct io_system *io_system
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
int io_system_destroy(
    struct io_system **io_system
);
