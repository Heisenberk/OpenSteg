/* Inclusions minimales. */
#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include "cmocka.h"

/* Inclusions supplémentaires. */
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include "stegx.h"
#include "common.h"

void free_infos(info_s * infos)
{
    if (infos->host.host != NULL)
        fclose(infos->host.host);
    if (infos->res != NULL)
        fclose(infos->res);
    free(infos->hidden_name);
    free(infos->passwd);
    free(infos);
    free(stegx_propos_algos);
}

void test_insert_eof_bmp_with_passwd(void **state)
{
	(void)state;
	stegx_choices_s* choices_insert=malloc(sizeof(stegx_choices_s));
	choices_insert->host_path=malloc((strlen("../../../env/test/test9bis.bmp")+1)*sizeof(char));
	strcpy(choices_insert->host_path,"../../../env/test/test9bis.bmp");
	choices_insert->res_path=malloc((strlen("./short.txt")+1)*sizeof(char));
	strcpy(choices_insert->res_path,"./res1_test_eof.bmp");
	choices_insert->passwd=malloc((strlen("stegx")+1)*sizeof(char));
	strcpy(choices_insert->passwd,"stegx");
	choices_insert->mode=STEGX_MODE_INSERT;
	choices_insert->insert_info=malloc(sizeof(stegx_info_insert_s));
	choices_insert->insert_info->hidden_path=malloc((strlen("../../../env/test/short.txt")+1)*sizeof(char));
	strcpy(choices_insert->insert_info->hidden_path,"../../../env/test/short.txt");
	choices_insert->insert_info->algo=STEGX_ALGO_EOF;
	int test;
	
	info_s *infos_insert = stegx_init(choices_insert);
	
	test=stegx_check_compatibility(infos_insert);
	assert_int_equal(test,0);
	
	test = stegx_suggest_algo(infos_insert);
	assert_int_equal(test,0);
	
	test=stegx_choose_algo(infos_insert, choices_insert->insert_info->algo);
	assert_int_equal(test,0);
	
	test=stegx_insert(infos_insert);
	assert_int_equal(test,0);
	
	stegx_clear(infos_insert);
	
	stegx_choices_s* choices_extract=malloc(sizeof(stegx_choices_s));
	choices_extract->host_path=malloc((strlen("./res1_test_eof.bmp")+1)*sizeof(char));
	strcpy(choices_extract->host_path,"./res1_test_eof.bmp");
	choices_extract->res_path=malloc((strlen("./")+1)*sizeof(char));
	strcpy(choices_extract->res_path,"./");
	choices_extract->passwd=malloc((strlen("stegx")+1)*sizeof(char));
	strcpy(choices_extract->passwd,"stegx");
	choices_extract->mode=STEGX_MODE_EXTRACT;
	choices_extract->insert_info=NULL;

	info_s *infos_extract = stegx_init(choices_extract);	

	test=stegx_check_compatibility(infos_extract);
	assert_int_equal(test,0);
	
	test=stegx_detect_algo(infos_extract);
	assert_int_equal(test,0);
	uint32_t length_malloc=infos_extract->hidden_length;
	test=stegx_extract(infos_extract,choices_extract->res_path);
	assert_int_equal(test,0);
	
	stegx_clear(infos_extract);
	
    uint8_t c;
    uint32_t i;
	char *message = malloc((length_malloc + 1) * sizeof(char));
	FILE *f = fopen("./short.txt", "r");
	assert_int_equal(f!=NULL,1);
    for (i = 0; i < (length_malloc) - 1; i++) {
        fread(&c, sizeof(uint8_t), 1, f);
        message[i] = c;
    }
    message[length_malloc - 2] = '\0';
    // Test si le contenu du message a bien ete extrait
    test = (strcmp(message, "voici un test tres court.") == 0);
    assert_int_equal(test, 1);
    free(message);
	
    remove("./short.txt");
    
}

void test_insert_eof_bmp_without_passwd(void **state)
{
	(void)state;
	stegx_choices_s* choices_insert=malloc(sizeof(stegx_choices_s));
	choices_insert->host_path=malloc((strlen("../../../env/test/test9bis.bmp")+1)*sizeof(char));
	strcpy(choices_insert->host_path,"../../../env/test/test9bis.bmp");
	choices_insert->res_path=malloc((strlen("./short.txt")+1)*sizeof(char));
	strcpy(choices_insert->res_path,"./res2_test_eof.bmp");
	choices_insert->passwd=NULL;
	choices_insert->mode=STEGX_MODE_INSERT;
	choices_insert->insert_info=malloc(sizeof(stegx_info_insert_s));
	choices_insert->insert_info->hidden_path=malloc((strlen("../../../env/test/short.txt")+1)*sizeof(char));
	strcpy(choices_insert->insert_info->hidden_path,"../../../env/test/short.txt");
	choices_insert->insert_info->algo=STEGX_ALGO_EOF;
	int test;
	
	info_s *infos_insert = stegx_init(choices_insert);
	
	test=stegx_check_compatibility(infos_insert);
	assert_int_equal(test,0);
	
	test = stegx_suggest_algo(infos_insert);
	assert_int_equal(test,0);
	
	test=stegx_choose_algo(infos_insert, choices_insert->insert_info->algo);
	assert_int_equal(test,0);
	
	test=stegx_insert(infos_insert);
	assert_int_equal(test,0);
	
	stegx_clear(infos_insert);
	
	stegx_choices_s* choices_extract=malloc(sizeof(stegx_choices_s));
	choices_extract->host_path=malloc((strlen("./res2_test_eof.bmp")+1)*sizeof(char));
	strcpy(choices_extract->host_path,"./res2_test_eof.bmp");
	choices_extract->res_path=malloc((strlen("./")+1)*sizeof(char));
	strcpy(choices_extract->res_path,"./");
	choices_extract->passwd=NULL;
	choices_extract->mode=STEGX_MODE_EXTRACT;
	choices_extract->insert_info=NULL;

	info_s *infos_extract = stegx_init(choices_extract);	

	test=stegx_check_compatibility(infos_extract);
	assert_int_equal(test,0);
	
	test=stegx_detect_algo(infos_extract);
	assert_int_equal(test,0);
	uint32_t length_malloc=infos_extract->hidden_length;
	test=stegx_extract(infos_extract,choices_extract->res_path);
	assert_int_equal(test,0);
	
	stegx_clear(infos_extract);
	
    uint8_t c;
    uint32_t i;
	char *message = malloc((length_malloc + 1) * sizeof(char));
	FILE *f = fopen("./short.txt", "r");
	assert_int_equal(f!=NULL,1);
    for (i = 0; i < (length_malloc) - 1; i++) {
        fread(&c, sizeof(uint8_t), 1, f);
        message[i] = c;
    }
    message[length_malloc - 2] = '\0';
    // Test si le contenu du message a bien ete extrait
    test = (strcmp(message, "voici un test tres court.") == 0);
    assert_int_equal(test, 1);
    free(message);
	
    remove("./short.txt");
}

void test_insert_eof_png_with_passwd(void **state)
{	(void)state;
	stegx_choices_s* choices_insert=malloc(sizeof(stegx_choices_s));
	choices_insert->host_path=malloc((strlen("../../../env/test/test8.png")+1)*sizeof(char));
	strcpy(choices_insert->host_path,"../../../env/test/test8.png");
	choices_insert->res_path=malloc((strlen("./short.txt")+1)*sizeof(char));
	strcpy(choices_insert->res_path,"./res3_test_eof.png");
	choices_insert->passwd=malloc((strlen("stegx")+1)*sizeof(char));
	strcpy(choices_insert->passwd,"stegx");
	choices_insert->mode=STEGX_MODE_INSERT;
	choices_insert->insert_info=malloc(sizeof(stegx_info_insert_s));
	choices_insert->insert_info->hidden_path=malloc((strlen("../../../env/test/short.txt")+1)*sizeof(char));
	strcpy(choices_insert->insert_info->hidden_path,"../../../env/test/short.txt");
	choices_insert->insert_info->algo=STEGX_ALGO_EOF;
	int test;
	
	info_s *infos_insert = stegx_init(choices_insert);
	
	test=stegx_check_compatibility(infos_insert);
	assert_int_equal(test,0);
	
	test = stegx_suggest_algo(infos_insert);
	assert_int_equal(test,0);
	
	test=stegx_choose_algo(infos_insert, choices_insert->insert_info->algo);
	assert_int_equal(test,0);
	
	test=stegx_insert(infos_insert);
	assert_int_equal(test,0);
	
	stegx_clear(infos_insert);
	
	stegx_choices_s* choices_extract=malloc(sizeof(stegx_choices_s));
	choices_extract->host_path=malloc((strlen("./res3_test_eof.png")+1)*sizeof(char));
	strcpy(choices_extract->host_path,"./res3_test_eof.png");
	choices_extract->res_path=malloc((strlen("./")+1)*sizeof(char));
	strcpy(choices_extract->res_path,"./");
	choices_extract->passwd=malloc((strlen("stegx")+1)*sizeof(char));
	strcpy(choices_extract->passwd,"stegx");
	choices_extract->mode=STEGX_MODE_EXTRACT;
	choices_extract->insert_info=NULL;

	info_s *infos_extract = stegx_init(choices_extract);	

	test=stegx_check_compatibility(infos_extract);
	assert_int_equal(test,0);
	
	test=stegx_detect_algo(infos_extract);
	assert_int_equal(test,0);
	uint32_t length_malloc=infos_extract->hidden_length;
	test=stegx_extract(infos_extract,choices_extract->res_path);
	assert_int_equal(test,0);
	
	stegx_clear(infos_extract);
	
    uint8_t c;
    uint32_t i;
	char *message = malloc((length_malloc + 1) * sizeof(char));
	FILE *f = fopen("./short.txt", "r");
	assert_int_equal(f!=NULL,1);
    for (i = 0; i < (length_malloc) - 1; i++) {
        fread(&c, sizeof(uint8_t), 1, f);
        message[i] = c;
    }
    message[length_malloc - 2] = '\0';
    // Test si le contenu du message a bien ete extrait
    test = (strcmp(message, "voici un test tres court.") == 0);
    assert_int_equal(test, 1);
    free(message);
	
    remove("./short.txt");

}

void test_insert_eof_png_without_passwd(void **state)
{
	(void)state;
	stegx_choices_s* choices_insert=malloc(sizeof(stegx_choices_s));
	choices_insert->host_path=malloc((strlen("../../../env/test/test8.png")+1)*sizeof(char));
	strcpy(choices_insert->host_path,"../../../env/test/test8.png");
	choices_insert->res_path=malloc((strlen("./short.txt")+1)*sizeof(char));
	strcpy(choices_insert->res_path,"./res4_test_eof.png");
	choices_insert->passwd=NULL;
	choices_insert->mode=STEGX_MODE_INSERT;
	choices_insert->insert_info=malloc(sizeof(stegx_info_insert_s));
	choices_insert->insert_info->hidden_path=malloc((strlen("../../../env/test/short.txt")+1)*sizeof(char));
	strcpy(choices_insert->insert_info->hidden_path,"../../../env/test/short.txt");
	choices_insert->insert_info->algo=STEGX_ALGO_EOF;
	int test;
	
	info_s *infos_insert = stegx_init(choices_insert);
	
	test=stegx_check_compatibility(infos_insert);
	assert_int_equal(test,0);
	
	test = stegx_suggest_algo(infos_insert);
	assert_int_equal(test,0);
	
	test=stegx_choose_algo(infos_insert, choices_insert->insert_info->algo);
	assert_int_equal(test,0);
	
	test=stegx_insert(infos_insert);
	assert_int_equal(test,0);
	
	stegx_clear(infos_insert);
	
	stegx_choices_s* choices_extract=malloc(sizeof(stegx_choices_s));
	choices_extract->host_path=malloc((strlen("./res4_test_eof.png")+1)*sizeof(char));
	strcpy(choices_extract->host_path,"./res4_test_eof.png");
	choices_extract->res_path=malloc((strlen("./")+1)*sizeof(char));
	strcpy(choices_extract->res_path,"./");
	choices_extract->passwd=NULL;
	choices_extract->mode=STEGX_MODE_EXTRACT;
	choices_extract->insert_info=NULL;

	info_s *infos_extract = stegx_init(choices_extract);	

	test=stegx_check_compatibility(infos_extract);
	assert_int_equal(test,0);
	
	test=stegx_detect_algo(infos_extract);
	assert_int_equal(test,0);
	uint32_t length_malloc=infos_extract->hidden_length;
	test=stegx_extract(infos_extract,choices_extract->res_path);
	assert_int_equal(test,0);
	
	stegx_clear(infos_extract);
	
    uint8_t c;
    uint32_t i;
	char *message = malloc((length_malloc + 1) * sizeof(char));
	FILE *f = fopen("./short.txt", "r");
	assert_int_equal(f!=NULL,1);
    for (i = 0; i < (length_malloc) - 1; i++) {
        fread(&c, sizeof(uint8_t), 1, f);
        message[i] = c;
    }
    message[length_malloc - 2] = '\0';
    // Test si le contenu du message a bien ete extrait
    test = (strcmp(message, "voici un test tres court.") == 0);
    assert_int_equal(test, 1);
    free(message);
	
    remove("./short.txt");
}

/* Structure CMocka contenant la liste des tests. */
const struct CMUnitTest insert_eof_tests[] = {
	cmocka_unit_test(test_insert_eof_bmp_with_passwd),
	cmocka_unit_test(test_insert_eof_bmp_without_passwd),
	cmocka_unit_test(test_insert_eof_png_with_passwd),
	cmocka_unit_test(test_insert_eof_png_without_passwd)
};

int main(void)
{
    /* Exécute les tests. */
    return cmocka_run_group_tests(insert_eof_tests, NULL, NULL);
}