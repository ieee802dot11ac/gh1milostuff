#include "argparse.h"
#include "converters.h"
#include "programinfo.h"
#include <stdlib.h>

int parse_args(int const argc, char const *const *const argv, HXConverterArgs *result)
{
	result->inputPath = NULL;
	result->outputPath = NULL;
	result->inputFileType = IFILETYPE_UNKNOWN;
	result->outputFileType = OFILETYPE_UNKNOWN;

	bool acceptPosixArgs = true;

	int skipArgs = 0;
	for (int i = 1; i < argc; ++i) {
		if (skipArgs > 0) {
			--skipArgs;
			continue;
		}
		char const *const arg = argv[i];
		if (acceptPosixArgs) {
			if (streq(arg, "--")) {
				acceptPosixArgs = false;
			} else if (streq(arg, "--help") || streq(arg, "-h")) {
				print_help(argv[0], stdout);
				return EXIT_SUCCESS;
			} else if (streq(arg, "--input") || streq(arg, "-i")) {
				if (i == argc - 1) {
					fprintf(stderr, "Not enough arguments to `%s`.\n", arg);
					return EXIT_FAILURE;
				}
				if ((result->inputFileType = get_input_filetype_arg(argv[i + 1])) == IFILETYPE_UNKNOWN) {
					fprintf(stderr, "Unimplemented input filetype: `%s`.\n", arg);
					return EXIT_FAILURE;
				}
				skipArgs = 1;
			} else if (streq(arg, "--output") || streq(arg, "-o")) {
				if (i == argc - 1) {
					fprintf(stderr, "Not enough arguments to `%s`.\n", arg);
					return EXIT_FAILURE;
				}
				if ((result->outputFileType = get_output_filetype_arg(argv[i + 1])) == OFILETYPE_UNKNOWN) {
					fprintf(stderr, "Unimplemented output filetype: `%s`.\n", arg);
					return EXIT_FAILURE;
				}
				skipArgs = 1;
			} else {
				goto ACCEPT_PATHS;
			}
			continue;
		}
ACCEPT_PATHS:
		if (result->inputPath == NULL) {
			result->inputPath = arg;
		} else if (result->outputPath == NULL) {
			result->outputPath = arg;
		} else {
			fputs("Too many arguments provided.\n", stderr);
			print_help(argv[0], stderr);
			return EXIT_FAILURE;
		}
	}

	if (result->inputPath == NULL) {
		fputs("No input or output file provided.\n\n", stderr);

		print_help(argv[0], stderr);
		return EXIT_FAILURE;
	} else if (result->outputPath == NULL) {
		fputs("No output file provided.\n\n", stderr);

		print_help(argv[0], stderr);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int guess_filetypes_if_unknown(HXConverterArgs *result)
{
	if (result->inputFileType == IFILETYPE_UNKNOWN) {
		size_t extensionLength = 0;
		for (size_t i = strlen(result->inputPath) - 1; i > 0; --i) {
			if (result->inputPath[i] == '.')
				break;
			++extensionLength;
		}
		char *const extension = malloc(sizeof(char) * (extensionLength + 1));
		strcpy(extension, &result->inputPath[strlen(result->inputPath) - extensionLength]);
		if ((result->inputFileType = get_input_filetype_ext(extension)) == IFILETYPE_UNKNOWN) {
			fprintf(stderr, "Unknown input file extension `.%s`\n", extension);
			free(extension);
			return EXIT_FAILURE;
		}
		free(extension);
	}

	if (result->outputFileType == OFILETYPE_UNKNOWN) {
		size_t extensionLength = 0;
		for (size_t i = strlen(result->outputPath) - 1; i > 0; --i) {
			if (result->outputPath[i] == '.')
				break;
			++extensionLength;
		}
		char *const extension = malloc(sizeof(char) * (extensionLength + 1));
		strcpy(extension, &result->outputPath[strlen(result->outputPath) - extensionLength]);
		if ((result->outputFileType = get_output_filetype_ext(extension)) == OFILETYPE_UNKNOWN) {
			fprintf(stderr, "Unknown output file extension `.%s`\n", extension);
			free(extension);
			return EXIT_FAILURE;
		}
		free(extension);
	}

	return EXIT_SUCCESS;
}

SUPPORTED_INPUT_FILETYPE get_input_filetype_arg(char const *const arg)
{
	if (streq(arg, "hmxmesh") || streq(arg, "hxm")) {
		return IFILETYPE_HX_MESH;
	} else if (streq(arg, "hmxtex") || streq(arg, "hxt")) {
		return IFILETYPE_HX_TEX;
	} else if (streq(arg, "hmxlit") || streq(arg, "hxl")) {
		return IFILETYPE_HX_LIT;
	} else if (streq(arg, "hmxcam") || streq(arg, "hxc")) {
		return IFILETYPE_HX_CAM;
	} else if (streq(arg, "mat") || streq(arg, "hxmat")) {
		return IFILETYPE_HX_MAT;
	}
	return IFILETYPE_UNKNOWN;
}

SUPPORTED_OUTPUT_FILETYPE get_output_filetype_arg(char const *const arg)
{
	if (streq(arg, "obj") || streq(arg, "wavefront")) {
		return OFILETYPE_WAVEFRONT_OBJ;
	} else if (streq(arg, "pam")) {
		return OFILETYPE_NETPBM_PAM;
	} else if (streq(arg, "png")) {
		return OFILETYPE_PNG;
	} else if (streq(arg, "mtl")) {
		return OFILETYPE_WAVEFRONT_MTL;
	}
	return OFILETYPE_UNKNOWN;
}

SUPPORTED_INPUT_FILETYPE get_input_filetype_ext(char const *const ext)
{
	if (streq(ext, "mesh") || streq(ext, "hxmesh")) {
		return IFILETYPE_HX_MESH;
	} else if (streq(ext, "tex") || streq(ext, "hxtex")) {
		return IFILETYPE_HX_TEX;
	} else if (streq(ext, "lit") || streq(ext, "hxlight")) {
		return IFILETYPE_HX_LIT;
	} else if (streq(ext, "cam") || streq(ext, "hxc")) {
		return IFILETYPE_HX_CAM;
	} else if (streq(ext, "mat") || streq(ext, "hxmat")) {
		return IFILETYPE_HX_MAT;
	}
	return IFILETYPE_UNKNOWN;
}

SUPPORTED_OUTPUT_FILETYPE get_output_filetype_ext(char const *const ext)
{
	if (streq(ext, "obj")) {
		return OFILETYPE_WAVEFRONT_OBJ;
	} else if (streq(ext, "pam")) {
		return OFILETYPE_NETPBM_PAM;
	} else if (streq(ext, "png")) {
		return OFILETYPE_PNG;
	} else if (streq(ext, "mtl")) {
		return OFILETYPE_WAVEFRONT_MTL;
	}
	return OFILETYPE_UNKNOWN;
}

void print_help(char const *const fileName, FILE *const writeTo)
{
	fputs(PROGRAM_NAME PROGRAM_VERSION"\n", writeTo);

	fprintf(writeTo, "%s [-h | --help]: Show this help text.\n", fileName);
	fprintf(writeTo, "%s [-i | --input <inputtype>] [-o | --output <outputtype>] <input> <output>: Convert files. If -i or -o aren't provided, guess corresponding file type from extension.\n", fileName);
}

size_t fsize(FILE *file)
{
	size_t curr = ftell(file);
	fseek(file, 0, SEEK_END);
	size_t len = ftell(file);
	fseek(file, curr, SEEK_SET);
	return len;
}
