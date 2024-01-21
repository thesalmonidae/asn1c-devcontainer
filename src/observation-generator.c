#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <cjson/cJSON.h>
#include "generated/Observation.h"

void validate_command_line_arguments(int argc, char* argv[]) {
    if (argc == 1) {
        printf("--input <json file> missing\n");
        exit(1);
    }

    if (argc == 2 && strcmp(argv[1], "--input") != 0) {
        printf("unknown flag %s\n", argv[1]);
        exit(2);
    }

    if (argc == 2 && strcmp(argv[1], "--input") == 0) {
        printf("no value for --input\n");
        exit(3);
    }

    if (argc > 3) {
        printf("extra arguments not allowed for --input\n");
        exit(4);
    }
}

static int write_file(const void *buffer, size_t size, void *app_key) {
    FILE *out_file_pointer = app_key;
    size_t wrote = fwrite(buffer, 1, size, out_file_pointer);
    return (wrote == size) ? 0 : -1;
}

int main(int argc, char* argv[]) {
    const char *observation_xml_filename = "observation.xml";
    const char *observation_der_filename = "observation.der";

    validate_command_line_arguments(argc, argv);

    char *work_directory_buffer;
    work_directory_buffer = (char *) malloc(1024 * sizeof(char));
    getcwd(work_directory_buffer, 1024);

    char filepath[1024];
    sprintf(filepath, "%s/%s", work_directory_buffer, argv[2]);

    FILE *json_file_pointer = fopen(filepath, "r"); 
    if (json_file_pointer == NULL) { 
        printf("could not open file %s\n", filepath); 
        exit(5);
    }

    printf("opened file %s\n", filepath);

    char json_buffer[1024];
    int length = fread(json_buffer, 1, sizeof(json_buffer), json_file_pointer);
    printf("read %d bytes from file %s\n", length, filepath);

    fclose(json_file_pointer);
    printf("closed file %s\n", filepath);

    cJSON *json_data = cJSON_Parse(json_buffer);
    if (json_data == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            printf("could not parse json data %s\n", error_ptr);
        }
        cJSON_Delete(json_data);
        exit(6);
    }

    printf("parsed json data\n");

    cJSON *x = cJSON_GetObjectItemCaseSensitive(json_data, "x");
    if (!cJSON_IsNumber(x)) {
        printf("x is not a number: %f", x->valuedouble);
        exit(7);
    }

    cJSON *y = cJSON_GetObjectItemCaseSensitive(json_data, "y");
    if (!cJSON_IsNumber(y)) {
        printf("y is not a number: %f", y->valuedouble);
        exit(7);
    }

    cJSON *z = cJSON_GetObjectItemCaseSensitive(json_data, "z");
    if (!cJSON_IsNumber(z)) {
        printf("z is not a number: %f", z->valuedouble);
        exit(7);
    }

    cJSON *timestamp = cJSON_GetObjectItemCaseSensitive(json_data, "timestamp");
    if (!cJSON_IsNumber(timestamp)) {
        printf("timestamp is not a number: %d", timestamp->valueint);
        exit(7);
    }

    printf("retrieved all fields from JSON data\n");

    Observation_t *observation;
    asn_enc_rval_t encoder_return_value;

    observation = calloc(1, sizeof(Observation_t));

    if (!observation) {
        perror("calloc failed for observation");
    }

    observation->x = x->valuedouble;
    observation->y = y->valuedouble;
    observation->z = z->valuedouble;
    observation->timestamp = timestamp->valueint;

    FILE *xer_file_pointer = fopen(observation_xml_filename, "w");

    if(!xer_file_pointer) {
        perror(observation_xml_filename);
        exit(1);
    }

    encoder_return_value = xer_encode(&asn_DEF_Observation, observation, XER_F_BASIC,  write_file, xer_file_pointer);

    fclose(xer_file_pointer);

    if(encoder_return_value.encoded == -1) {
        fprintf(stderr, "could not encode Observation (at %s)\n",
        encoder_return_value.failed_type ? encoder_return_value.failed_type->name : "unknown");
        exit(1);
    } else {
        fprintf(stderr, "wrote file %s with XER encoded Observation\n", observation_xml_filename);
    }

    FILE *der_file_pointer = fopen(observation_der_filename, "w");

    if(!der_file_pointer) {
        perror(observation_der_filename);
        exit(1);
    }

    encoder_return_value = der_encode(&asn_DEF_Observation, observation, write_file, der_file_pointer);

    fclose(der_file_pointer);

    if(encoder_return_value.encoded == -1) {
        fprintf(stderr, "could not encode Observation (at %s)\n",
        encoder_return_value.failed_type ? encoder_return_value.failed_type->name : "unknown");
        exit(1);
    } else {
        fprintf(stderr, "wrote file %s with DER encoded Observation\n", observation_der_filename);
    }

    return 0;
}
