#include <vaccel.h>
#include <ops/vaccel_ops.h>
#include <stddef.h>
#include <sys/ioctl.h>
#include <string.h>
#include <accel.h>

#include "ioctl.h"
#include "log.h"
#include "session.h"

int virtio_noop(struct vaccel_session *sess)
{
	enum vaccel_op_type op_type = VACCEL_NO_OP;
	struct accel_session vsess = { 0 };
	struct accel_arg arg = { sizeof(op_type), (char *)&op_type };

	vsess.id = sess->session_id;
	vsess.op.out_nr = 1;
	vsess.op.out = &arg;

	vaccel_debug("[virtio] session:%u Executing noop",
			sess->session_id);

	return dev_write(VACCEL_DO_OP, &vsess);
}

int virtio_sgemm(struct vaccel_session *sess, uint32_t k, uint32_t m,
		uint32_t n, size_t len_a, size_t len_b, size_t len_c,
		float *a, float *b, float *c)
{
	enum vaccel_op_type op_type = VACCEL_BLAS_SGEMM;
	struct accel_session vsess = { 0 };
	struct accel_arg args[7] = {
		{ sizeof(op_type), (char *)&op_type },
		{ sizeof(k), (char *)&k },
		{ sizeof(m), (char *)&m },
		{ sizeof(n), (char *)&n },
		{ len_a, (unsigned char *)a },
		{ len_b, (unsigned char *)b },
		{ len_c, (unsigned char *)c },
	};

	vsess.id = sess->session_id;
	vsess.op.out_nr = 6;
	vsess.op.out = args;
	vsess.op.in_nr = 1;
	vsess.op.in = &args[6];

	vaccel_debug("[virtio] session:%u Executing sgemm",
			sess->session_id);

	return dev_write(VACCEL_DO_OP, &vsess);
}

int virtio_image_classification(struct vaccel_session *sess, void *img,
		char *out_text, char *out_imgname,
		size_t len_img, size_t len_out_text, size_t len_out_imgname)
{
	enum vaccel_op_type op_type = VACCEL_IMG_CLASS;
	struct accel_session vsess = { 0 };
	struct accel_arg args[4] = {
		{ sizeof(op_type), (char *)&op_type },
		{ len_img, img },
		{ len_out_text, out_text },
		{ len_out_imgname, out_imgname },
	};

	vsess.id = sess->session_id;
	vsess.op.out_nr = 2;
	vsess.op.out = &args[0];
	vsess.op.in_nr = 2;
	vsess.op.in = &args[2];

	vaccel_debug("[virtio] session:%u Executing image classification",
			sess->session_id);

	return dev_write(VACCEL_DO_OP, &vsess);
}

int virtio_image_detection(struct vaccel_session *sess, void *img,
		char *out_imgname, size_t len_img, size_t len_out_imgname)
{
	enum vaccel_op_type op_type = VACCEL_IMG_DETEC;
	struct accel_session vsess = { 0 };
	struct accel_arg args[3] = {
		{ sizeof(op_type), (char *)&op_type },
		{ len_img, img },
		{ len_out_imgname, out_imgname },
	};

	vsess.id = sess->session_id;
	vsess.op.out_nr = 2;
	vsess.op.out = &args[0];
	vsess.op.in_nr = 1;
	vsess.op.in = &args[2];

	vaccel_debug("[virtio] session:%u Executing image detection",
			sess->session_id);

	return dev_write(VACCEL_DO_OP, &vsess);
}

int virtio_image_segmentation(struct vaccel_session *sess, void *img,
		char *out_imgname, size_t len_img, size_t len_out_imgname)
{
	enum vaccel_op_type op_type = VACCEL_IMG_SEGME;
	struct accel_session vsess = { 0 };
	struct accel_arg args[3] = {
		{ sizeof(op_type), (char *)&op_type },
		{ len_img, img },
		{ len_out_imgname, out_imgname },
	};

	vsess.id = sess->session_id;
	vsess.op.out_nr = 2;
	vsess.op.out = &args[0];
	vsess.op.in_nr = 1;
	vsess.op.in = &args[2];

	vaccel_debug("[virtio] session:%u Executing image segmentation",
			sess->session_id);

	return dev_write(VACCEL_DO_OP, &vsess);
}

int virtio_exec(struct vaccel_session *sess, const char *library, const char
		*fn_symbol, void *out_args, size_t out_nargs, void *in_args,
		size_t in_nargs)
{
        enum vaccel_op_type op_type = VACCEL_EXEC;
        struct accel_session vsess = { 0 };
        struct accel_arg args[out_nargs + in_nargs + 3];
        int i = 0, idx = 0;

        /* pass VACCEL_EXEC */
        args[idx].len = sizeof(op_type);
        args[idx].buf = (char*)&op_type;
	idx++;

        /* pass first argument */
        args[idx].len = strlen(library);
        args[idx].buf = (char*)library;
	idx++;

        /* pass second argument */
        args[idx].len = strlen(fn_symbol);
        args[idx].buf = (char*)fn_symbol;
	idx++;

        /* pass the rest of the read-only arguments */
        for (i = 0; i < out_nargs;i++){
                args[idx].len = ((struct accel_arg*)out_args)[i].len;
                args[idx].buf = ((struct accel_arg*)out_args)[i].buf;
		idx++;
        }

        /* pass the write-only arguments */
        for (i =0; i < in_nargs;i++){
                args[idx].len = ((struct accel_arg*)in_args)[i].len;
                args[idx].buf = ((struct accel_arg*)in_args)[i].buf;
		idx++;
        }
 	
        vsess.id = sess->session_id;
        vsess.op.out_nr = out_nargs + 3; /* Read-only args; */
        vsess.op.out = &args[0];
        vsess.op.in_nr = in_nargs; /* Write-only args; */
        vsess.op.in = &args[out_nargs + 3];

        vaccel_debug("[virtio] session:%u Executing exec",
                        sess->session_id);

        return dev_write(VACCEL_DO_OP, &vsess);
}

