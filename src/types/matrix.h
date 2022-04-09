#ifndef MAT_H
#define MAT_H

#include "../common.h"

mat*
matrixInit(int data_type_in, size_t rows, size_t columns, const char* options) {

	if(	data_type_in == 0 ||
		rows == 0 ||
		columns == 0) {
			setError(ARG_ERR, FN, "Input argument(s) invalid: data_type_in, rows, and columns must be nonzero");
			return NULL;
	}

	if(data_type_in == FLT_TYPE) { // check if dimensions are too large
		if(rows * (columns / 1024.) > 131072) { // 131072 = 1048576 / 8
			setError(ARG_ERR, FN, "Input argument(s) invalid: rows * columns exceeds max size for FLT_TYPE (~ 1GB)");
			return NULL;
		}	
	} else {
		if(rows * (columns / 1024.) > 262144) { // 262144 = 1048576 / 4
			setError(ARG_ERR, FN, "Input argument(s) invalid: rows * columns exceeds max size for ITR_TYPE (~ 1GB)");
			return NULL;
		}
	}

	mat* output_mat = NULL;
	if(!(output_mat = malloc(sizeof(mat)))) {
		fatalExit(FN, "Allocate failed for: output_mat (malloc)");
	}
	output_mat->type = MAT_TYPE;
	output_mat->datatype = data_type_in;
	output_mat->m = rows;
	output_mat->n = columns;

	switch(data_type_in) {

		case ITR_TYPE: {
			if(!(output_mat->data = calloc(rows * columns, sizeof(int32_t)))) {
				free(output_mat);
				fatalExit(FN, "Allocate failed for: output_mat->data (malloc)");
			}
			if(seqSame(options, "unit")) {
				for(size_t i = 0; i < rows * columns; i++) {
					((int32_t *)output_mat->data)[i] = 1;
				}
			}
			break;
		}

		case FLT_TYPE: {
			if(!(output_mat->data = calloc(rows * columns, sizeof(double)))) {
				free(output_mat);
				fatalExit(FN, "Allocate failed for: output_mat->data (malloc)");
			}
			if(seqSame(options, "unit")) {
				for(size_t i = 0; i < rows * columns; i++) {
					((double *)output_mat->data)[i] = 1;
				}
			}
			break;
		}
		default: {
			setError(ARG_ERR, FN, "Argument invalid: data_type_in (matrixInit expects ITR_TYPE or FLT_TYPE)");
			free(output_mat);
			return NULL;
		}
	}
	setSuccess(FN);
	return output_mat;
}

int
destroyMatrix(mat* mat_in) {

	if(mat_in == NULL) {
		setError(ARG_ERR, FN, "Input object invalid (NULL)");
		return 1;
	}

	if(mat_in->data) {
		free(mat_in->data);
	}
	free(mat_in);

	setSuccess(FN);
	return 0;
}

mat*
duplicateMatrix(mat* mat_in) {

	if(mat_in == NULL) {
		setError(ARG_ERR, FN, "Input object invalid (NULL)");
		return NULL;
	}
	if(mat_in->data == NULL) {
		setError(ARG_ERR, FN, "Input object invalid: mat_in->data = NULL");
		return NULL;
	}

	mat* mat_out = NULL;
	if(!(mat_out = matrixInit(mat_in->datatype, mat_in->m, mat_in->n, NULL))) {
		setError(INIT_ERR, FN, "Allocate failed for: mat_out (matrixInit)");
		return NULL;
	}

	size_t total_data_len = mat_out->m * mat_out->n;

	switch(mat_out->datatype) {

		case ITR_TYPE: {
			for(size_t i = 0; i < total_data_len; i++) {
				((int *)mat_out->data)[i] = ((int *)mat_in->data)[i];
			}
			break;
		}

		case FLT_TYPE: {
			for(size_t i = 0; i < total_data_len; i++) {
				((double *)mat_out->data)[i] = ((double *)mat_in->data)[i];
			}
			break;
		}
	}

	setSuccess(FN);
	return mat_out;
}

#endif // MAT_H
