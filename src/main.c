/*
 * main.c
 *
 *  Created on: 2014-1-1
 *      Author: li
 */

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <nginx.h>
int rtn;

#define _EXE_(exe) rtn=exe;if(NGX_OK!=rtn) return rtn;
#define _INI_(dst,alloc) dst=alloc;if(dst==NULL) return NGX_ERROR;


int main(int argc, char **argv) {
	ngx_log_t        *log;
	ngx_pool_t       *pool;
	u_char      *ngx_prefix="./";

	_INI_(log, ngx_log_init(ngx_prefix))
	_EXE_(init(log))
    _INI_(pool,ngx_create_pool(1024,log))

    ngx_log_error(NGX_LOG_CRIT, log, 0, "===============testArray================");
    _EXE_(testArray(pool,log));
    ngx_log_error(NGX_LOG_CRIT, log, 0, "===============testAlloc================");
    _EXE_(testAlloc(pool,log));
    ngx_log_error(NGX_LOG_CRIT, log, 0, "===============testChain================");
    _EXE_(testChain(pool,log));
    ngx_log_error(NGX_LOG_CRIT, log, 0, "===============testLog  ================");
    _EXE_(testLog(pool,log));
    ngx_log_error(NGX_LOG_CRIT, log, 0, "===============testTimer================");
    _EXE_(testTimer(pool,log));

    ngx_log_error(NGX_LOG_CRIT, log, 0, "begin destroy pool");
	ngx_destroy_pool(pool);

}
int init(ngx_log_t *log){
	_EXE_ (ngx_strerror_init())//初始化errorList
	ngx_time_init();           //初始化时间，否则日志没有时间
	ngx_pid = ngx_getpid();    //初始化时间，否则日志没有pid

	/*{//可能需要ngx_log_abort时使用，目前ngx_log_abort不通
		ngx_cycle_t      init_cycle;//在
	    ngx_memzero(&init_cycle, sizeof(ngx_cycle_t));
	    init_cycle.log = log;

	    _INI_(init_cycle.pool , ngx_create_pool(256, log))

	    ngx_cycle = &init_cycle;
	}*/

    return 0;
}
int _testArray_push2Array(ngx_pool_t *pool,ngx_array_t* uris,ngx_str_t *path){

    ngx_str_t  * uri ;
    _INI_(uri,ngx_array_push(uris))

    uri->len = path->len;
    _INI_(uri->data ,ngx_pnalloc(pool, uri->len ))

    u_char * d=ngx_cpymem(uri->data, path->data, path->len);


    return 0;
}
int testArray(ngx_pool_t *pool,ngx_log_t        *log){
    ngx_array_t                *uris1;
    ngx_array_t                 uris2;

    ngx_memzero(&uris2, sizeof(ngx_array_t));

    _EXE_(ngx_array_init(&uris2, pool, 8, sizeof(ngx_str_t)) )
    _INI_(uris1,ngx_array_create(pool, 3, sizeof(ngx_str_t)) )

    _EXE_(_testArray1(pool,uris1,log))
    _EXE_(_testArray1(pool,&uris2,log))

    ngx_array_destroy(uris1);
    ngx_array_destroy(&uris2);

    return 0;
}

int _testArray1(ngx_pool_t *pool,ngx_array_t* uris,ngx_log_t        *log){
    int iFor1;

    ngx_str_t str1 = ngx_string("hello world1");
    ngx_str_t str2 = ngx_string("hello world2");
    ngx_str_t str3 = ngx_string("hello world3");
    ngx_str_t str4 = ngx_string("hello world4");
    ngx_str_t str5 = ngx_string("hello world5");
    _EXE_(_testArray_push2Array(pool,uris,&str1));
    _EXE_(_testArray_push2Array(pool,uris,&str2));
    _EXE_(_testArray_push2Array(pool,uris,&str3));
    _EXE_(_testArray_push2Array(pool,uris,&str4));
    _EXE_(_testArray_push2Array(pool,uris,&str5));

	ngx_str_t  *uri=uris->elts;
    ngx_log_error(NGX_LOG_CRIT, log, 0, "array size: %d,nalloc:%d",uris->size,uris->nalloc);
    for (iFor1 = 0; iFor1 < uris->nelts; iFor1++) {
        ngx_log_error(NGX_LOG_CRIT, log, 0, "[%d]: \"%V\"",iFor1, uri+iFor1);
	}
    return 0;
}

void _testAlloc_clean_up(void *data){
	ngx_log_t        *log=data;
    ngx_log_error(NGX_LOG_CRIT, log, 0, "just test clean up，this will call when pool release");
}
int testAlloc(ngx_pool_t *pool,ngx_log_t        *log){
	void * p1;
	void * p2;
	void * p3;
	_INI_(p1,ngx_palloc(pool,10))
	_INI_(p2,ngx_pcalloc(pool,10))
	_INI_(p3,ngx_pnalloc(pool,10))

	ngx_pool_cleanup_t * cleanup=ngx_pool_cleanup_add(pool,0);
	cleanup->data=log;
	cleanup->handler=_testAlloc_clean_up;
    return 0;
}

void _testChain_print(ngx_pool_t *pool,ngx_log_t *log,ngx_chain_t *next_chain){
    ngx_log_error(NGX_LOG_CRIT, log, 0, "_testChain_print");
    int iFor1=0;
	ngx_str_t str;
    while(next_chain){
    	if(ngx_buf_in_memory(next_chain->buf)){
    		ngx_buf_t *buf=next_chain->buf;
        	str.data=buf->pos;
        	str.len=buf->last-buf->pos;

            ngx_log_error(NGX_LOG_CRIT, log, 0, "pos:%p buf:%p %d",buf->pos,buf,str.len);
            ngx_log_error(NGX_LOG_CRIT, log, 0, "[%d]:%d \"%V\"",iFor1,str.len,&str);
    	}
        next_chain=next_chain->next;
        iFor1++;
    }
}

int _testChain_push(ngx_pool_t *pool,ngx_chain_t ** now,ngx_str_t *str,ngx_log_t *log){

	//1
	ngx_buf_t *buf = ngx_pcalloc(pool, sizeof(ngx_buf_t));
    if (buf == NULL) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }
    //2
    //ngx_buf_t *buf =ngx_calloc_buf(pool);
    //3
    //ngx_buf_t *buf =ngx_alloc_buf(pool);

    buf->pos = str->data;
    buf->last = buf->pos + str->len;
    buf->memory = 1;

    ngx_buf_size(buf);

    ngx_log_error(NGX_LOG_CRIT, log, 0, "pos:%p buf:%p %d",buf->pos,buf,str->len);

    ngx_chain_t *p_chain = ngx_alloc_chain_link(pool);
    p_chain->buf = buf;
    p_chain->next = *now;
    *now=p_chain;

    return 0;
}


int testChain(ngx_pool_t *pool,ngx_log_t *log){
	ngx_chain_t                *out=NULL;
	ngx_str_t                   str5;


    ngx_str_t str1 = ngx_string("hello world1");
    ngx_str_t str2 = ngx_string("hello world2");
    ngx_str_t str3 = ngx_string("hello world3");
    ngx_str_t str4 = ngx_string("hello world4");
    ngx_str_set(&str5, "hello world5");
    _EXE_(_testChain_push(pool,&out,&str1,log))
    _EXE_(_testChain_push(pool,&out,&str2,log))
    _EXE_(_testChain_push(pool,&out,&str3,log))
    _EXE_(_testChain_push(pool,&out,&str4,log))
    _EXE_(_testChain_push(pool,&out,&str5,log))

    _testChain_print(pool,log,out);

//	ngx_free_chain(pool,out);

    return 0;
}

int testLog(ngx_pool_t *pool,ngx_log_t *log){

    ngx_log_error(NGX_LOG_CRIT , log, 0,"log level:%d", log->log_level);

//    ngx_log_error_core(NGX_LOG_STDERR, log, 0, "ngx_log_debug_core（NGX_LOG_STDERR:%d）",NGX_LOG_STDERR);
    ngx_log_error_core(NGX_LOG_EMERG , log, 0, "ngx_log_debug_core（NGX_LOG_EMERG :%d）",NGX_LOG_EMERG );
    ngx_log_error_core(NGX_LOG_ALERT , log, 0, "ngx_log_debug_core（NGX_LOG_ALERT :%d）",NGX_LOG_ALERT );
    ngx_log_error_core(NGX_LOG_CRIT  , log, 0, "ngx_log_debug_core（NGX_LOG_CRIT  :%d）",NGX_LOG_CRIT  );
    ngx_log_error_core(NGX_LOG_ERR   , log, 0, "ngx_log_debug_core（NGX_LOG_ERR   :%d）",NGX_LOG_ERR   );
    ngx_log_error_core(NGX_LOG_WARN  , log, 0, "ngx_log_debug_core（NGX_LOG_WARN  :%d）",NGX_LOG_WARN  );
    ngx_log_error_core(NGX_LOG_NOTICE, log, 0, "ngx_log_debug_core（NGX_LOG_NOTICE:%d）",NGX_LOG_NOTICE);
    ngx_log_error_core(NGX_LOG_INFO  , log, 0, "ngx_log_debug_core（NGX_LOG_INFO  :%d）",NGX_LOG_INFO  );
    ngx_log_error_core(NGX_LOG_DEBUG , log, 0, "ngx_log_debug_core（NGX_LOG_DEBUG :%d）",NGX_LOG_DEBUG );

    /*ngx_log_debug_core( log, 0, "ngx_log_debug_core（NGX_LOG_STDERR:%d）",NGX_LOG_STDERR);
    ngx_log_debug_core( log, 0, "ngx_log_debug_core（NGX_LOG_EMERG :%d）",NGX_LOG_EMERG );
    ngx_log_debug_core( log, 0, "ngx_log_debug_core（NGX_LOG_ALERT :%d）",NGX_LOG_ALERT );
    ngx_log_debug_core( log, 0, "ngx_log_debug_core（NGX_LOG_CRIT  :%d）",NGX_LOG_CRIT  );
    ngx_log_debug_core( log, 0, "ngx_log_debug_core（NGX_LOG_ERR   :%d）",NGX_LOG_ERR   );
    ngx_log_debug_core( log, 0, "ngx_log_debug_core（NGX_LOG_WARN  :%d）",NGX_LOG_WARN  );
    ngx_log_debug_core( log, 0, "ngx_log_debug_core（NGX_LOG_NOTICE:%d）",NGX_LOG_NOTICE);
    ngx_log_debug_core( log, 0, "ngx_log_debug_core（NGX_LOG_INFO  :%d）",NGX_LOG_INFO  );
    ngx_log_debug_core( log, 0, "ngx_log_debug_core（NGX_LOG_DEBUG :%d）",NGX_LOG_DEBUG );*/

//    ngx_log_error(NGX_LOG_STDERR, log, 0, "ngx_log_error（NGX_LOG_STDERR:%d）",NGX_LOG_STDERR);
    ngx_log_error(NGX_LOG_EMERG , log, 0, "ngx_log_error（NGX_LOG_EMERG :%d）",NGX_LOG_EMERG );
    ngx_log_error(NGX_LOG_ALERT , log, 0, "ngx_log_error（NGX_LOG_ALERT :%d）",NGX_LOG_ALERT );
    ngx_log_error(NGX_LOG_CRIT  , log, 0, "ngx_log_error（NGX_LOG_CRIT  :%d）",NGX_LOG_CRIT  );
    ngx_log_error(NGX_LOG_ERR   , log, 0, "ngx_log_error（NGX_LOG_ERR   :%d）",NGX_LOG_ERR   );
    ngx_log_error(NGX_LOG_WARN  , log, 0, "ngx_log_error（NGX_LOG_WARN  :%d）",NGX_LOG_WARN  );
    ngx_log_error(NGX_LOG_NOTICE, log, 0, "ngx_log_error（NGX_LOG_NOTICE:%d）",NGX_LOG_NOTICE);
    ngx_log_error(NGX_LOG_INFO  , log, 0, "ngx_log_error（NGX_LOG_INFO  :%d）",NGX_LOG_INFO  );
    ngx_log_error(NGX_LOG_DEBUG , log, 0, "ngx_log_error（NGX_LOG_DEBUG :%d）",NGX_LOG_DEBUG );
    /*
    ngx_log_abort(0, "ngx_log_abort（NGX_LOG_STDERR:%s）","NGX_LOG_STDERR");
    ngx_log_abort(0, "ngx_log_abort（NGX_LOG_EMERG :%s）","NGX_LOG_EMERG ");
    ngx_log_abort(0, "ngx_log_abort（NGX_LOG_ALERT :%s）","NGX_LOG_ALERT ");
    ngx_log_abort(0, "ngx_log_abort（NGX_LOG_CRIT  :%s）","NGX_LOG_CRIT  ");
    ngx_log_abort(0, "ngx_log_abort（NGX_LOG_ERR   :%s）","NGX_LOG_ERR   ");
    ngx_log_abort(0, "ngx_log_abort（NGX_LOG_WARN  :%s）","NGX_LOG_WARN  ");
    ngx_log_abort(0, "ngx_log_abort（NGX_LOG_NOTICE:%s）","NGX_LOG_NOTICE");
    ngx_log_abort(0, "ngx_log_abort（NGX_LOG_INFO  :%s）","NGX_LOG_INFO  ");
    ngx_log_abort(0, "ngx_log_abort（NGX_LOG_DEBUG :%s）","NGX_LOG_DEBUG ");*/

    ngx_log_debug1(NGX_LOG_EMERG , log, 0, "ngx_log_debug1（%d）                     ",1);
    ngx_log_debug2(NGX_LOG_ALERT , log, 0, "ngx_log_debug2（%d,%d）                  ",1,1);
    ngx_log_debug3(NGX_LOG_CRIT  , log, 0, "ngx_log_debug3（%d,%d,%d）               ",1,1,1);
    ngx_log_debug4(NGX_LOG_ERR   , log, 0, "ngx_log_debug4（%d,%d,%d,%d）            ",1,1,1,1);
    ngx_log_debug5(NGX_LOG_WARN  , log, 0, "ngx_log_debug5（%d,%d,%d,%d,%d）         ",1,1,1,1,1);
    ngx_log_debug6(NGX_LOG_NOTICE, log, 0, "ngx_log_debug6（%d,%d,%d,%d,%d,%d）      ",1,1,1,1,1,1);
    ngx_log_debug7(NGX_LOG_INFO  , log, 0, "ngx_log_debug7（%d,%d,%d,%d,%d,%d,%d）   ",1,1,1,1,1,1,1);
    ngx_log_debug8(NGX_LOG_DEBUG , log, 0, "ngx_log_debug8（%d,%d,%d,%d,%d,%d,%d,%d）",1,1,1,1,1,1,1,1);

    ngx_log_stderr(0,"ngx_log_stderr");
    return 0;
}

static ngx_connection_t dummy;
static ngx_event_t ev;
static int count=1;

static void _testTimer_print(ngx_event_t *ev)
{
	ngx_log_error(NGX_LOG_CRIT, ev->log, 0, "_testTimer_print\n");
    if(count < 10){
    	count=count+1;
    	ngx_add_timer(ev, 1000);
    }
}


void _testTimer_init(ngx_log_t *log)
{
    dummy.fd = (ngx_socket_t) -1;

    ngx_memzero(&ev, sizeof(ngx_event_t));

    ev.handler = _testTimer_print;
    ev.log = log;
    ev.data = &dummy;

    ngx_add_timer(&ev, 1000);

    while(count<10)
    {

    }
}
int testTimer(ngx_pool_t *pool,ngx_log_t *log){
	_testTimer_init(log);
	return NGX_OK;
}
