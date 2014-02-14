NGINX_SRC=../../tengine
all:main epoll timer

main:main.c
	gcc -g -o main  main.c \
	-I ${NGINX_SRC}/src/core \
	-I ${NGINX_SRC}/src/event \
	-I ${NGINX_SRC}/src/event/modules \
	-I ${NGINX_SRC}/src/os/unix \
	-I ${NGINX_SRC}/src/proc \
	-I ${NGINX_SRC}/objs \
	-I ${NGINX_SRC}/src/http \
	-I ${NGINX_SRC}/src/http/modules \
	-I ${NGINX_SRC}/src/mail \
	${NGINX_SRC}/objs/src/core/nginx.o \
	${NGINX_SRC}/objs/src/core/ngx_log.o \
	${NGINX_SRC}/objs/src/core/ngx_palloc.o \
	${NGINX_SRC}/objs/src/core/ngx_array.o \
	${NGINX_SRC}/objs/src/core/ngx_list.o \
	${NGINX_SRC}/objs/src/core/ngx_hash.o \
	${NGINX_SRC}/objs/src/core/ngx_buf.o \
	${NGINX_SRC}/objs/src/core/ngx_queue.o \
	${NGINX_SRC}/objs/src/core/ngx_output_chain.o \
	${NGINX_SRC}/objs/src/core/ngx_string.o \
	${NGINX_SRC}/objs/src/core/ngx_parse.o \
	${NGINX_SRC}/objs/src/core/ngx_inet.o \
	${NGINX_SRC}/objs/src/core/ngx_file.o \
	${NGINX_SRC}/objs/src/core/ngx_crc32.o \
	${NGINX_SRC}/objs/src/core/ngx_murmurhash.o \
	${NGINX_SRC}/objs/src/core/ngx_md5.o \
	${NGINX_SRC}/objs/src/core/ngx_rbtree.o \
	${NGINX_SRC}/objs/src/core/ngx_trie.o \
	${NGINX_SRC}/objs/src/core/ngx_segment_tree.o \
	${NGINX_SRC}/objs/src/core/ngx_radix_tree.o \
	${NGINX_SRC}/objs/src/core/ngx_slab.o \
	${NGINX_SRC}/objs/src/core/ngx_times.o \
	${NGINX_SRC}/objs/src/core/ngx_shmtx.o \
	${NGINX_SRC}/objs/src/core/ngx_connection.o \
	${NGINX_SRC}/objs/src/core/ngx_cycle.o \
	${NGINX_SRC}/objs/src/core/ngx_spinlock.o \
	${NGINX_SRC}/objs/src/core/ngx_cpuinfo.o \
	${NGINX_SRC}/objs/src/core/ngx_conf_file.o \
	${NGINX_SRC}/objs/src/core/ngx_resolver.o \
	${NGINX_SRC}/objs/src/core/ngx_open_file_cache.o \
	${NGINX_SRC}/objs/src/core/ngx_crypt.o \
	${NGINX_SRC}/objs/src/event/ngx_event.o \
	${NGINX_SRC}/objs/src/event/ngx_event_timer.o \
	${NGINX_SRC}/objs/src/event/ngx_event_posted.o \
	${NGINX_SRC}/objs/src/event/ngx_event_busy_lock.o \
	${NGINX_SRC}/objs/src/event/ngx_event_accept.o \
	${NGINX_SRC}/objs/src/event/ngx_event_connect.o \
	${NGINX_SRC}/objs/src/event/ngx_event_pipe.o \
	${NGINX_SRC}/objs/src/os/unix/ngx_time.o \
	${NGINX_SRC}/objs/src/os/unix/ngx_errno.o \
	${NGINX_SRC}/objs/src/os/unix/ngx_alloc.o \
	${NGINX_SRC}/objs/src/os/unix/ngx_files.o \
	${NGINX_SRC}/objs/src/os/unix/ngx_socket.o \
	${NGINX_SRC}/objs/src/os/unix/ngx_recv.o \
	${NGINX_SRC}/objs/src/os/unix/ngx_readv_chain.o \
	${NGINX_SRC}/objs/src/os/unix/ngx_udp_recv.o \
	${NGINX_SRC}/objs/src/os/unix/ngx_send.o \
	${NGINX_SRC}/objs/src/os/unix/ngx_writev_chain.o \
	${NGINX_SRC}/objs/src/os/unix/ngx_channel.o \
	${NGINX_SRC}/objs/src/os/unix/ngx_shmem.o \
	${NGINX_SRC}/objs/src/os/unix/ngx_process.o \
	${NGINX_SRC}/objs/src/os/unix/ngx_daemon.o \
	${NGINX_SRC}/objs/src/os/unix/ngx_setproctitle.o \
	${NGINX_SRC}/objs/src/os/unix/ngx_posix_init.o \
	${NGINX_SRC}/objs/src/os/unix/ngx_user.o \
	${NGINX_SRC}/objs/src/os/unix/ngx_pipe.o \
	${NGINX_SRC}/objs/src/os/unix/ngx_sysinfo.o \
	${NGINX_SRC}/objs/src/os/unix/ngx_process_cycle.o \
	${NGINX_SRC}/objs/src/os/unix/ngx_linux_init.o \
	${NGINX_SRC}/objs/src/event/modules/ngx_epoll_module.o \
	${NGINX_SRC}/objs/src/os/unix/ngx_linux_sendfile_chain.o \
	${NGINX_SRC}/objs/src/os/unix/ngx_syslog.o \
	${NGINX_SRC}/objs/src/core/ngx_dso_module.o \
	${NGINX_SRC}/objs/src/proc/ngx_proc.o \
	${NGINX_SRC}/objs/src/event/ngx_event_openssl.o \
	${NGINX_SRC}/objs/src/event/ngx_event_openssl_stapling.o \
	${NGINX_SRC}/objs/src/core/ngx_regex.o \
	${NGINX_SRC}/objs/src/http/ngx_http.o \
	${NGINX_SRC}/objs/src/http/ngx_http_core_module.o \
	${NGINX_SRC}/objs/src/http/ngx_http_special_response.o \
	${NGINX_SRC}/objs/src/http/ngx_http_request.o \
	${NGINX_SRC}/objs/src/http/ngx_http_parse.o \
	${NGINX_SRC}/objs/src/http/ngx_http_header_filter_module.o \
	${NGINX_SRC}/objs/src/http/ngx_http_write_filter_module.o \
	${NGINX_SRC}/objs/src/http/ngx_http_copy_filter_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_log_module.o \
	${NGINX_SRC}/objs/src/http/ngx_http_request_body.o \
	${NGINX_SRC}/objs/src/http/ngx_http_variables.o \
	${NGINX_SRC}/objs/src/http/ngx_http_script.o \
	${NGINX_SRC}/objs/src/http/ngx_http_upstream.o \
	${NGINX_SRC}/objs/src/http/ngx_http_upstream_round_robin.o \
	${NGINX_SRC}/objs/src/http/ngx_http_parse_time.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_static_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_index_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_chunked_filter_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_range_filter_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_headers_filter_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_not_modified_filter_module.o \
	${NGINX_SRC}/objs/src/http/ngx_http_busy_lock.o \
	${NGINX_SRC}/objs/src/http/ngx_http_file_cache.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_gzip_filter_module.o \
	${NGINX_SRC}/objs/src/http/ngx_http_postpone_filter_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_ssi_filter_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_charset_filter_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_userid_filter_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_footer_filter_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_trim_filter_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_autoindex_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_auth_basic_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_access_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_limit_conn_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_limit_req_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_geo_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_map_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_split_clients_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_referer_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_rewrite_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_ssl_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_proxy_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_fastcgi_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_uwsgi_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_scgi_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_memcached_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_empty_gif_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_browser_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_user_agent_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_upstream_ip_hash_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_upstream_consistent_hash_module.o \
	${NGINX_SRC}/objs/src/http/ngx_http_upstream_check_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_upstream_least_conn_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_upstream_session_sticky_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_upstream_keepalive_module.o \
	${NGINX_SRC}/objs/src/http/modules/ngx_http_stub_status_module.o \
	${NGINX_SRC}/objs/ngx_modules.o \
	-lpthread -ldl -lcrypt -lpcre -lssl -lcrypto -ldl -lz
	mkdir -p logs
clean:
	rm main
epoll:epoll.c
	gcc -Wall -o epoll epoll.c
timer:timer.c
	gcc -Wall -o timer timer.c