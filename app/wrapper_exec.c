#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <vaccel.h>

int fpga_sample1()
{
	int ret = 0;
	struct vaccel_session sess;

        ret = vaccel_sess_init(&sess, 0);
        if (ret != VACCEL_OK) {
                fprintf(stderr, "Could not initialize session\n");
                return 1;
        }

        printf("Initialized session with id: %u\n", sess.session_id);

	//char *library = "opencl_examples/build/vector_add/libvector_add.so";
        char *library = "fpga_example/build/sample/libsample1.so";
        char *operation = "sample1";

        ret = vaccel_exec(&sess, library, operation, NULL, 0, NULL, 0);
	if (ret) {
		fprintf(stderr, "Could not run op: %d\n", ret);
		goto close_session;
	}

close_session:
        if (vaccel_sess_free(&sess) != VACCEL_OK) {
                fprintf(stderr, "Could not clear session\n");
                return 1;
        }

	return ret;
}

int sample1()
{
	fpga_sample1();

	return 0;
}
