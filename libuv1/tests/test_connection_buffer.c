#include <string.h>
#include <assert.h>

#include "tests/main.h"
#include "connection.h"

void test_connection_buffer(void)
{
    // Test allocation and deallocation
    {
        ConnectionPtr cnx = connection_new();
        for(int i = 0; i < 5; i++)
        {
            connection_allocate_buffer(cnx, sizeof(int));
        }
        connection_destroy(cnx);
    }

    // Test message buffered in one chunk
    {
        ConnectionPtr cnx = connection_new();
        char *input = "qwe\n";
        void *buffer = connection_allocate_buffer(cnx, strlen(input) * sizeof(char));
        memcpy(buffer, input, strlen(input) * sizeof(char));
        char *output = connection_get_message(cnx);
        assert(strcmp(output, "qwe") == 0);
        connection_cycle_buffers(cnx);
        free(output);
        connection_destroy(cnx);
    }

    // Test message buffered in one chunk with buffer reuse
    {
        ConnectionPtr cnx = connection_new();
        char *input1 = "abc1\n";
        char *input2 = "def2\n";
        char *output = NULL;
        void *buffer = NULL;

        // 1
        buffer = connection_allocate_buffer(cnx, strlen(input1) * sizeof(char));
        memcpy(buffer, input1, strlen(input1) * sizeof(char));
        output = connection_get_message(cnx);
        assert(strcmp(output, "abc1") == 0);
        connection_cycle_buffers(cnx);
        free(output);

        // 2
        buffer = connection_allocate_buffer(cnx, strlen(input2) * sizeof(char));
        memcpy(buffer, input2, strlen(input2) * sizeof(char));
        output = connection_get_message(cnx);
        assert(strcmp(output, "def2") == 0);
        connection_cycle_buffers(cnx);
        free(output);

        connection_destroy(cnx);
    }

    // Test message buffered in multiple chunks
    {
        ConnectionPtr cnx = connection_new();
        char *input1 = "abc";
        char *input2 = "def1\n";
        void *buffer = connection_allocate_buffer(cnx, strlen(input1) * sizeof(char));
        memcpy(buffer, input1, strlen(input1) * sizeof(char));
        buffer = connection_allocate_buffer(cnx, strlen(input2) * sizeof(char));
        memcpy(buffer, input2, strlen(input2) * sizeof(char));
        char *output = connection_get_message(cnx);
        assert(strcmp(output, "abcdef1") == 0);
        connection_cycle_buffers(cnx);
        free(output);
        connection_destroy(cnx);
    }

    // Test message buffered in multiple chunks with buffer reuse
    {
        ConnectionPtr cnx = connection_new();
        char *input1_1 = "abc";
        char *input1_2 = "def1\n";
        char *input2_1 = "ghi";
        char *input2_2 = "jkl2\n";
        void *buffer = NULL;
        char *output = NULL;

        // 1
        buffer = connection_allocate_buffer(cnx, strlen(input1_1) * sizeof(char));
        memcpy(buffer, input1_1, strlen(input1_1) * sizeof(char));
        buffer = connection_allocate_buffer(cnx, strlen(input1_2) * sizeof(char));
        memcpy(buffer, input1_2, strlen(input1_2) * sizeof(char));
        output = connection_get_message(cnx);
        assert(strcmp(output, "abcdef1") == 0);
        connection_cycle_buffers(cnx);
        free(output);

        // 2
        buffer = connection_allocate_buffer(cnx, strlen(input2_1) * sizeof(char));
        memcpy(buffer, input2_1, strlen(input2_1) * sizeof(char));
        buffer = connection_allocate_buffer(cnx, strlen(input2_2) * sizeof(char));
        memcpy(buffer, input2_2, strlen(input2_2) * sizeof(char));
        output = connection_get_message(cnx);
        assert(strcmp(output, "ghijkl2") == 0);
        connection_cycle_buffers(cnx);
        free(output);

        connection_destroy(cnx);
    }
}

RUN_TEST(test_connection_buffer)
