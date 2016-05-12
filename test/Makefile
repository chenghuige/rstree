#COMAKE2 edit-mode: -*- Makefile -*-
####################64Bit Mode####################
ifeq ($(shell uname -m),x86_64)
CC=g++
CXX=g++
CXXFLAGS=-g \
  -O3 \
  -pipe \
  -W \
  -Wall \
  -fPIC \
  -DHAVE_NETINET_IN_H \
  -Wno-unused-parameter \
  -Wno-deprecated \
  -std=c++11 \
  -fpermissive \
  -Wno-write-strings \
  -Wno-literal-suffix \
  -Wno-unused-local-typedefs
CFLAGS=-g \
  -O3 \
  -pipe \
  -W \
  -Wall \
  -fPIC \
  -DHAVE_NETINET_IN_H \
  -Wno-unused-parameter \
  -Wno-deprecated \
  -std=c++11 \
  -fpermissive \
  -Wno-write-strings \
  -Wno-literal-suffix \
  -Wno-unused-local-typedefs
CPPFLAGS=-D_GNU_SOURCE \
  -D__STDC_LIMIT_MACROS \
  -DVERSION=\"1.9.8.7\"
INCPATH=-I../include \
  -I../include/post_process
DEP_INCPATH=-I../../../../../../app/search/sep/anti-spam/gezi \
  -I../../../../../../app/search/sep/anti-spam/gezi/include \
  -I../../../../../../app/search/sep/anti-spam/gezi/output \
  -I../../../../../../app/search/sep/anti-spam/gezi/output/include \
  -I../../../../../../com/btest/gtest \
  -I../../../../../../com/btest/gtest/include \
  -I../../../../../../com/btest/gtest/output \
  -I../../../../../../com/btest/gtest/output/include \
  -I../../../../../../com/idlcompiler \
  -I../../../../../../com/idlcompiler/include \
  -I../../../../../../com/idlcompiler/output \
  -I../../../../../../com/idlcompiler/output/include \
  -I../../../../../../lib2-64/bsl \
  -I../../../../../../lib2-64/bsl/include \
  -I../../../../../../lib2-64/bsl/output \
  -I../../../../../../lib2-64/bsl/output/include \
  -I../../../../../../lib2-64/ccode \
  -I../../../../../../lib2-64/ccode/include \
  -I../../../../../../lib2-64/ccode/output \
  -I../../../../../../lib2-64/ccode/output/include \
  -I../../../../../../lib2-64/dict \
  -I../../../../../../lib2-64/dict/include \
  -I../../../../../../lib2-64/dict/output \
  -I../../../../../../lib2-64/dict/output/include \
  -I../../../../../../lib2-64/libcrf \
  -I../../../../../../lib2-64/libcrf/include \
  -I../../../../../../lib2-64/libcrf/output \
  -I../../../../../../lib2-64/libcrf/output/include \
  -I../../../../../../lib2-64/others-ex \
  -I../../../../../../lib2-64/others-ex/include \
  -I../../../../../../lib2-64/others-ex/output \
  -I../../../../../../lib2-64/others-ex/output/include \
  -I../../../../../../lib2-64/postag \
  -I../../../../../../lib2-64/postag/include \
  -I../../../../../../lib2-64/postag/output \
  -I../../../../../../lib2-64/postag/output/include \
  -I../../../../../../lib2-64/ullib \
  -I../../../../../../lib2-64/ullib/include \
  -I../../../../../../lib2-64/ullib/output \
  -I../../../../../../lib2-64/ullib/output/include \
  -I../../../../../../lib2-64/wordseg \
  -I../../../../../../lib2-64/wordseg/include \
  -I../../../../../../lib2-64/wordseg/output \
  -I../../../../../../lib2-64/wordseg/output/include \
  -I../../../../../../op/oped/noah/webfoot/naming-lib \
  -I../../../../../../op/oped/noah/webfoot/naming-lib/include \
  -I../../../../../../op/oped/noah/webfoot/naming-lib/output \
  -I../../../../../../op/oped/noah/webfoot/naming-lib/output/include \
  -I../../../../../../public/bslext \
  -I../../../../../../public/bslext/include \
  -I../../../../../../public/bslext/output \
  -I../../../../../../public/bslext/output/include \
  -I../../../../../../public/comlog-plugin \
  -I../../../../../../public/comlog-plugin/include \
  -I../../../../../../public/comlog-plugin/output \
  -I../../../../../../public/comlog-plugin/output/include \
  -I../../../../../../public/configure \
  -I../../../../../../public/configure/include \
  -I../../../../../../public/configure/output \
  -I../../../../../../public/configure/output/include \
  -I../../../../../../public/connectpool \
  -I../../../../../../public/connectpool/include \
  -I../../../../../../public/connectpool/output \
  -I../../../../../../public/connectpool/output/include \
  -I../../../../../../public/idlcompiler \
  -I../../../../../../public/idlcompiler/include \
  -I../../../../../../public/idlcompiler/output \
  -I../../../../../../public/idlcompiler/output/include \
  -I../../../../../../public/mcpack \
  -I../../../../../../public/mcpack/include \
  -I../../../../../../public/mcpack/output \
  -I../../../../../../public/mcpack/output/include \
  -I../../../../../../public/nshead \
  -I../../../../../../public/nshead/include \
  -I../../../../../../public/nshead/output \
  -I../../../../../../public/nshead/output/include \
  -I../../../../../../public/odict \
  -I../../../../../../public/odict/include \
  -I../../../../../../public/odict/output \
  -I../../../../../../public/odict/output/include \
  -I../../../../../../public/spreg \
  -I../../../../../../public/spreg/include \
  -I../../../../../../public/spreg/output \
  -I../../../../../../public/spreg/output/include \
  -I../../../../../../public/ub \
  -I../../../../../../public/ub/include \
  -I../../../../../../public/ub/output \
  -I../../../../../../public/ub/output/include \
  -I../../../../../../public/uconv \
  -I../../../../../../public/uconv/include \
  -I../../../../../../public/uconv/output \
  -I../../../../../../public/uconv/output/include \
  -I../../../../../../quality/autotest/reportlib/cpp \
  -I../../../../../../quality/autotest/reportlib/cpp/include \
  -I../../../../../../quality/autotest/reportlib/cpp/output \
  -I../../../../../../quality/autotest/reportlib/cpp/output/include \
  -I../../../../../../third-64/boost \
  -I../../../../../../third-64/boost/include \
  -I../../../../../../third-64/boost/output \
  -I../../../../../../third-64/boost/output/include \
  -I../../../../../../third-64/gflags \
  -I../../../../../../third-64/gflags/include \
  -I../../../../../../third-64/gflags/output \
  -I../../../../../../third-64/gflags/output/include \
  -I../../../../../../third-64/glog \
  -I../../../../../../third-64/glog/include \
  -I../../../../../../third-64/glog/output \
  -I../../../../../../third-64/glog/output/include \
  -I../../../../../../third-64/pcre \
  -I../../../../../../third-64/pcre/include \
  -I../../../../../../third-64/pcre/output \
  -I../../../../../../third-64/pcre/output/include \
  -I../../../../../../third-64/protobuf \
  -I../../../../../../third-64/protobuf/include \
  -I../../../../../../third-64/protobuf/output \
  -I../../../../../../third-64/protobuf/output/include \
  -I../../../../../../third-64/tcmalloc \
  -I../../../../../../third-64/tcmalloc/include \
  -I../../../../../../third-64/tcmalloc/output \
  -I../../../../../../third-64/tcmalloc/output/include

#============ CCP vars ============
CCHECK=@ccheck.py
CCHECK_FLAGS=
PCLINT=@pclint
PCLINT_FLAGS=
CCP=@ccp.py
CCP_FLAGS=


#COMAKE UUID
COMAKE_MD5=5d111dca1af5bd654b45281be60b7998  COMAKE


.PHONY:all
all:comake2_makefile_check test_post_processor test_rstree 
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mall[0m']"
	@echo "make all done"

.PHONY:comake2_makefile_check
comake2_makefile_check:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mcomake2_makefile_check[0m']"
	#in case of error, update 'Makefile' by 'comake2'
	@echo "$(COMAKE_MD5)">comake2.md5
	@md5sum -c --status comake2.md5
	@rm -f comake2.md5

.PHONY:ccpclean
ccpclean:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mccpclean[0m']"
	@echo "make ccpclean done"

.PHONY:clean
clean:ccpclean
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mclean[0m']"
	rm -rf test_post_processor
	rm -rf ./output/bin/test_post_processor
	rm -rf test_rstree
	rm -rf ./output/bin/test_rstree
	rm -rf test_post_processor_test_post_processor.o
	rm -rf ../src/test_post_processor_conf.o
	rm -rf ../src/post_process/test_post_processor_PostProcessor.o
	rm -rf ../src/post_process/test_post_processor_RstreeFilter.o
	rm -rf test_rstree_test_rstree.o
	rm -rf ../src/test_rstree_conf.o
	rm -rf ../src/post_process/test_rstree_PostProcessor.o
	rm -rf ../src/post_process/test_rstree_RstreeFilter.o

.PHONY:dist
dist:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mdist[0m']"
	tar czvf output.tar.gz output
	@echo "make dist done"

.PHONY:distclean
distclean:clean
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mdistclean[0m']"
	rm -f output.tar.gz
	@echo "make distclean done"

.PHONY:love
love:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mlove[0m']"
	@echo "make love done"

test_post_processor:test_post_processor_test_post_processor.o \
  ../src/test_post_processor_conf.o \
  ../src/post_process/test_post_processor_PostProcessor.o \
  ../src/post_process/test_post_processor_RstreeFilter.o
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mtest_post_processor[0m']"
	$(CXX) test_post_processor_test_post_processor.o \
  ../src/test_post_processor_conf.o \
  ../src/post_process/test_post_processor_PostProcessor.o \
  ../src/post_process/test_post_processor_RstreeFilter.o -Xlinker "-("  ../../../../../../app/search/sep/anti-spam/gezi/libgezi_common.a \
  ../../../../../../app/search/sep/anti-spam/gezi/libgezi_cppformat.a \
  ../../../../../../app/search/sep/anti-spam/gezi/libgezi_json.a \
  ../../../../../../app/search/sep/anti-spam/gezi/output/lib/libPYNotation.a \
  ../../../../../../com/btest/gtest/output/lib/libgtest.a \
  ../../../../../../com/btest/gtest/output/lib/libgtest_main.a \
  ../../../../../../com/idlcompiler/astyle/libastyle.a \
  ../../../../../../com/idlcompiler/cxx/libskeleton.a \
  ../../../../../../com/idlcompiler/java/libjava_skeleton.a \
  ../../../../../../com/idlcompiler/parser/libparser.a \
  ../../../../../../com/idlcompiler/php/libphp_skeleton.a \
  ../../../../../../lib2-64/bsl/lib/libbsl.a \
  ../../../../../../lib2-64/bsl/lib/libbsl_ResourcePool.a \
  ../../../../../../lib2-64/bsl/lib/libbsl_archive.a \
  ../../../../../../lib2-64/bsl/lib/libbsl_buffer.a \
  ../../../../../../lib2-64/bsl/lib/libbsl_check_cast.a \
  ../../../../../../lib2-64/bsl/lib/libbsl_exception.a \
  ../../../../../../lib2-64/bsl/lib/libbsl_pool.a \
  ../../../../../../lib2-64/bsl/lib/libbsl_utils.a \
  ../../../../../../lib2-64/bsl/lib/libbsl_var.a \
  ../../../../../../lib2-64/bsl/lib/libbsl_var_implement.a \
  ../../../../../../lib2-64/bsl/lib/libbsl_var_utils.a \
  ../../../../../../lib2-64/ccode/lib/libulccode.a \
  ../../../../../../lib2-64/dict/lib/libuldict.a \
  ../../../../../../lib2-64/libcrf/lib/libcrf.a \
  ../../../../../../lib2-64/others-ex/lib/libullib_ex.a \
  ../../../../../../lib2-64/postag/lib/libpostag.a \
  ../../../../../../lib2-64/ullib/lib/libullib.a \
  ../../../../../../lib2-64/wordseg/libsegment.a \
  ../../../../../../op/oped/noah/webfoot/naming-lib/output/libwebfoot_naming.a \
  ../../../../../../public/bslext/output/lib/libbsl_bml.a \
  ../../../../../../public/bslext/output/lib/libbsl_containers_utils.a \
  ../../../../../../public/bslext/output/lib/libbsl_var_scripting.a \
  ../../../../../../public/bslext/output/lib/libbsl_var_serializer.a \
  ../../../../../../public/bslext/output/lib/libbsl_var_vscript.a \
  ../../../../../../public/bslext/output/lib/libbsl_vs.a \
  ../../../../../../public/bslext/output/lib/libbslext.a \
  ../../../../../../public/comlog-plugin/libcomlog.a \
  ../../../../../../public/comlog-plugin/output/lib/libdfsappender.a \
  ../../../../../../public/configure/libconfig.a \
  ../../../../../../public/connectpool/libconnectpool.a \
  ../../../../../../public/idlcompiler/output/lib/libmcpack_idl.a \
  ../../../../../../public/mcpack/libmcpack.a \
  ../../../../../../public/nshead/libnshead.a \
  ../../../../../../public/odict/libodict.a \
  ../../../../../../public/spreg/libspreg.a \
  ../../../../../../public/ub/lib/libub.a \
  ../../../../../../public/ub/output/lib/libub_aserver.a \
  ../../../../../../public/ub/output/lib/libub_client.a \
  ../../../../../../public/ub/output/lib/libub_conf.a \
  ../../../../../../public/ub/output/lib/libub_galileo.a \
  ../../../../../../public/ub/output/lib/libub_log.a \
  ../../../../../../public/ub/output/lib/libub_misc.a \
  ../../../../../../public/ub/output/lib/libub_monitor.a \
  ../../../../../../public/ub/output/lib/libub_naming.a \
  ../../../../../../public/ub/output/lib/libub_server.a \
  ../../../../../../public/ub/output/lib/libubex.a \
  ../../../../../../public/ub/output/lib/libubfw.a \
  ../../../../../../public/uconv/libuconv.a \
  ../../../../../../quality/autotest/reportlib/cpp/libautotest.a \
  ../../../../../../third-64/boost/lib/libboost_atomic.a \
  ../../../../../../third-64/boost/lib/libboost_chrono.a \
  ../../../../../../third-64/boost/lib/libboost_container.a \
  ../../../../../../third-64/boost/lib/libboost_context.a \
  ../../../../../../third-64/boost/lib/libboost_coroutine.a \
  ../../../../../../third-64/boost/lib/libboost_date_time.a \
  ../../../../../../third-64/boost/lib/libboost_exception.a \
  ../../../../../../third-64/boost/lib/libboost_filesystem.a \
  ../../../../../../third-64/boost/lib/libboost_graph.a \
  ../../../../../../third-64/boost/lib/libboost_iostreams.a \
  ../../../../../../third-64/boost/lib/libboost_locale.a \
  ../../../../../../third-64/boost/lib/libboost_log.a \
  ../../../../../../third-64/boost/lib/libboost_log_setup.a \
  ../../../../../../third-64/boost/lib/libboost_math_c99.a \
  ../../../../../../third-64/boost/lib/libboost_math_c99f.a \
  ../../../../../../third-64/boost/lib/libboost_math_c99l.a \
  ../../../../../../third-64/boost/lib/libboost_math_tr1.a \
  ../../../../../../third-64/boost/lib/libboost_math_tr1f.a \
  ../../../../../../third-64/boost/lib/libboost_math_tr1l.a \
  ../../../../../../third-64/boost/lib/libboost_prg_exec_monitor.a \
  ../../../../../../third-64/boost/lib/libboost_program_options.a \
  ../../../../../../third-64/boost/lib/libboost_python.a \
  ../../../../../../third-64/boost/lib/libboost_random.a \
  ../../../../../../third-64/boost/lib/libboost_regex.a \
  ../../../../../../third-64/boost/lib/libboost_serialization.a \
  ../../../../../../third-64/boost/lib/libboost_signals.a \
  ../../../../../../third-64/boost/lib/libboost_system.a \
  ../../../../../../third-64/boost/lib/libboost_test_exec_monitor.a \
  ../../../../../../third-64/boost/lib/libboost_thread.a \
  ../../../../../../third-64/boost/lib/libboost_timer.a \
  ../../../../../../third-64/boost/lib/libboost_unit_test_framework.a \
  ../../../../../../third-64/boost/lib/libboost_wave.a \
  ../../../../../../third-64/boost/lib/libboost_wserialization.a \
  ../../../../../../third-64/gflags/lib/libgflags.a \
  ../../../../../../third-64/gflags/lib/libgflags_nothreads.a \
  ../../../../../../third-64/glog/lib/libglog.a \
  ../../../../../../third-64/pcre/lib/libpcre.a \
  ../../../../../../third-64/pcre/lib/libpcrecpp.a \
  ../../../../../../third-64/pcre/lib/libpcreposix.a \
  ../../../../../../third-64/protobuf/lib/libprotobuf-lite.a \
  ../../../../../../third-64/protobuf/lib/libprotobuf.a \
  ../../../../../../third-64/protobuf/lib/libprotoc.a \
  ../../../../../../third-64/tcmalloc/lib/libprofiler.a \
  ../../../../../../third-64/tcmalloc/lib/libtcmalloc.a \
  ../../../../../../third-64/tcmalloc/lib/libtcmalloc_and_profiler.a \
  ../../../../../../third-64/tcmalloc/lib/libtcmalloc_debug.a \
  ../../../../../../third-64/tcmalloc/lib/libtcmalloc_minimal.a \
  ../../../../../../third-64/tcmalloc/lib/libtcmalloc_minimal_debug.a -lpthread \
  -lcrypto \
  -lrt -Xlinker "-)" -o test_post_processor
	mkdir -p ./output/bin
	cp -f --link test_post_processor ./output/bin

test_rstree:test_rstree_test_rstree.o \
  ../src/test_rstree_conf.o \
  ../src/post_process/test_rstree_PostProcessor.o \
  ../src/post_process/test_rstree_RstreeFilter.o
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mtest_rstree[0m']"
	$(CXX) test_rstree_test_rstree.o \
  ../src/test_rstree_conf.o \
  ../src/post_process/test_rstree_PostProcessor.o \
  ../src/post_process/test_rstree_RstreeFilter.o -Xlinker "-("  ../../../../../../app/search/sep/anti-spam/gezi/libgezi_common.a \
  ../../../../../../app/search/sep/anti-spam/gezi/libgezi_cppformat.a \
  ../../../../../../app/search/sep/anti-spam/gezi/libgezi_json.a \
  ../../../../../../app/search/sep/anti-spam/gezi/output/lib/libPYNotation.a \
  ../../../../../../com/btest/gtest/output/lib/libgtest.a \
  ../../../../../../com/btest/gtest/output/lib/libgtest_main.a \
  ../../../../../../com/idlcompiler/astyle/libastyle.a \
  ../../../../../../com/idlcompiler/cxx/libskeleton.a \
  ../../../../../../com/idlcompiler/java/libjava_skeleton.a \
  ../../../../../../com/idlcompiler/parser/libparser.a \
  ../../../../../../com/idlcompiler/php/libphp_skeleton.a \
  ../../../../../../lib2-64/bsl/lib/libbsl.a \
  ../../../../../../lib2-64/bsl/lib/libbsl_ResourcePool.a \
  ../../../../../../lib2-64/bsl/lib/libbsl_archive.a \
  ../../../../../../lib2-64/bsl/lib/libbsl_buffer.a \
  ../../../../../../lib2-64/bsl/lib/libbsl_check_cast.a \
  ../../../../../../lib2-64/bsl/lib/libbsl_exception.a \
  ../../../../../../lib2-64/bsl/lib/libbsl_pool.a \
  ../../../../../../lib2-64/bsl/lib/libbsl_utils.a \
  ../../../../../../lib2-64/bsl/lib/libbsl_var.a \
  ../../../../../../lib2-64/bsl/lib/libbsl_var_implement.a \
  ../../../../../../lib2-64/bsl/lib/libbsl_var_utils.a \
  ../../../../../../lib2-64/ccode/lib/libulccode.a \
  ../../../../../../lib2-64/dict/lib/libuldict.a \
  ../../../../../../lib2-64/libcrf/lib/libcrf.a \
  ../../../../../../lib2-64/others-ex/lib/libullib_ex.a \
  ../../../../../../lib2-64/postag/lib/libpostag.a \
  ../../../../../../lib2-64/ullib/lib/libullib.a \
  ../../../../../../lib2-64/wordseg/libsegment.a \
  ../../../../../../op/oped/noah/webfoot/naming-lib/output/libwebfoot_naming.a \
  ../../../../../../public/bslext/output/lib/libbsl_bml.a \
  ../../../../../../public/bslext/output/lib/libbsl_containers_utils.a \
  ../../../../../../public/bslext/output/lib/libbsl_var_scripting.a \
  ../../../../../../public/bslext/output/lib/libbsl_var_serializer.a \
  ../../../../../../public/bslext/output/lib/libbsl_var_vscript.a \
  ../../../../../../public/bslext/output/lib/libbsl_vs.a \
  ../../../../../../public/bslext/output/lib/libbslext.a \
  ../../../../../../public/comlog-plugin/libcomlog.a \
  ../../../../../../public/comlog-plugin/output/lib/libdfsappender.a \
  ../../../../../../public/configure/libconfig.a \
  ../../../../../../public/connectpool/libconnectpool.a \
  ../../../../../../public/idlcompiler/output/lib/libmcpack_idl.a \
  ../../../../../../public/mcpack/libmcpack.a \
  ../../../../../../public/nshead/libnshead.a \
  ../../../../../../public/odict/libodict.a \
  ../../../../../../public/spreg/libspreg.a \
  ../../../../../../public/ub/lib/libub.a \
  ../../../../../../public/ub/output/lib/libub_aserver.a \
  ../../../../../../public/ub/output/lib/libub_client.a \
  ../../../../../../public/ub/output/lib/libub_conf.a \
  ../../../../../../public/ub/output/lib/libub_galileo.a \
  ../../../../../../public/ub/output/lib/libub_log.a \
  ../../../../../../public/ub/output/lib/libub_misc.a \
  ../../../../../../public/ub/output/lib/libub_monitor.a \
  ../../../../../../public/ub/output/lib/libub_naming.a \
  ../../../../../../public/ub/output/lib/libub_server.a \
  ../../../../../../public/ub/output/lib/libubex.a \
  ../../../../../../public/ub/output/lib/libubfw.a \
  ../../../../../../public/uconv/libuconv.a \
  ../../../../../../quality/autotest/reportlib/cpp/libautotest.a \
  ../../../../../../third-64/boost/lib/libboost_atomic.a \
  ../../../../../../third-64/boost/lib/libboost_chrono.a \
  ../../../../../../third-64/boost/lib/libboost_container.a \
  ../../../../../../third-64/boost/lib/libboost_context.a \
  ../../../../../../third-64/boost/lib/libboost_coroutine.a \
  ../../../../../../third-64/boost/lib/libboost_date_time.a \
  ../../../../../../third-64/boost/lib/libboost_exception.a \
  ../../../../../../third-64/boost/lib/libboost_filesystem.a \
  ../../../../../../third-64/boost/lib/libboost_graph.a \
  ../../../../../../third-64/boost/lib/libboost_iostreams.a \
  ../../../../../../third-64/boost/lib/libboost_locale.a \
  ../../../../../../third-64/boost/lib/libboost_log.a \
  ../../../../../../third-64/boost/lib/libboost_log_setup.a \
  ../../../../../../third-64/boost/lib/libboost_math_c99.a \
  ../../../../../../third-64/boost/lib/libboost_math_c99f.a \
  ../../../../../../third-64/boost/lib/libboost_math_c99l.a \
  ../../../../../../third-64/boost/lib/libboost_math_tr1.a \
  ../../../../../../third-64/boost/lib/libboost_math_tr1f.a \
  ../../../../../../third-64/boost/lib/libboost_math_tr1l.a \
  ../../../../../../third-64/boost/lib/libboost_prg_exec_monitor.a \
  ../../../../../../third-64/boost/lib/libboost_program_options.a \
  ../../../../../../third-64/boost/lib/libboost_python.a \
  ../../../../../../third-64/boost/lib/libboost_random.a \
  ../../../../../../third-64/boost/lib/libboost_regex.a \
  ../../../../../../third-64/boost/lib/libboost_serialization.a \
  ../../../../../../third-64/boost/lib/libboost_signals.a \
  ../../../../../../third-64/boost/lib/libboost_system.a \
  ../../../../../../third-64/boost/lib/libboost_test_exec_monitor.a \
  ../../../../../../third-64/boost/lib/libboost_thread.a \
  ../../../../../../third-64/boost/lib/libboost_timer.a \
  ../../../../../../third-64/boost/lib/libboost_unit_test_framework.a \
  ../../../../../../third-64/boost/lib/libboost_wave.a \
  ../../../../../../third-64/boost/lib/libboost_wserialization.a \
  ../../../../../../third-64/gflags/lib/libgflags.a \
  ../../../../../../third-64/gflags/lib/libgflags_nothreads.a \
  ../../../../../../third-64/glog/lib/libglog.a \
  ../../../../../../third-64/pcre/lib/libpcre.a \
  ../../../../../../third-64/pcre/lib/libpcrecpp.a \
  ../../../../../../third-64/pcre/lib/libpcreposix.a \
  ../../../../../../third-64/protobuf/lib/libprotobuf-lite.a \
  ../../../../../../third-64/protobuf/lib/libprotobuf.a \
  ../../../../../../third-64/protobuf/lib/libprotoc.a \
  ../../../../../../third-64/tcmalloc/lib/libprofiler.a \
  ../../../../../../third-64/tcmalloc/lib/libtcmalloc.a \
  ../../../../../../third-64/tcmalloc/lib/libtcmalloc_and_profiler.a \
  ../../../../../../third-64/tcmalloc/lib/libtcmalloc_debug.a \
  ../../../../../../third-64/tcmalloc/lib/libtcmalloc_minimal.a \
  ../../../../../../third-64/tcmalloc/lib/libtcmalloc_minimal_debug.a -lpthread \
  -lcrypto \
  -lrt -Xlinker "-)" -o test_rstree
	mkdir -p ./output/bin
	cp -f --link test_rstree ./output/bin

test_post_processor_test_post_processor.o:test_post_processor.cc
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mtest_post_processor_test_post_processor.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o test_post_processor_test_post_processor.o test_post_processor.cc

../src/test_post_processor_conf.o:../src/conf.cpp
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40m../src/test_post_processor_conf.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o ../src/test_post_processor_conf.o ../src/conf.cpp

../src/post_process/test_post_processor_PostProcessor.o:../src/post_process/PostProcessor.cpp
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40m../src/post_process/test_post_processor_PostProcessor.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o ../src/post_process/test_post_processor_PostProcessor.o ../src/post_process/PostProcessor.cpp

../src/post_process/test_post_processor_RstreeFilter.o:../src/post_process/RstreeFilter.cpp
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40m../src/post_process/test_post_processor_RstreeFilter.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o ../src/post_process/test_post_processor_RstreeFilter.o ../src/post_process/RstreeFilter.cpp

test_rstree_test_rstree.o:test_rstree.cc
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mtest_rstree_test_rstree.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o test_rstree_test_rstree.o test_rstree.cc

../src/test_rstree_conf.o:../src/conf.cpp
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40m../src/test_rstree_conf.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o ../src/test_rstree_conf.o ../src/conf.cpp

../src/post_process/test_rstree_PostProcessor.o:../src/post_process/PostProcessor.cpp
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40m../src/post_process/test_rstree_PostProcessor.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o ../src/post_process/test_rstree_PostProcessor.o ../src/post_process/PostProcessor.cpp

../src/post_process/test_rstree_RstreeFilter.o:../src/post_process/RstreeFilter.cpp
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40m../src/post_process/test_rstree_RstreeFilter.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o ../src/post_process/test_rstree_RstreeFilter.o ../src/post_process/RstreeFilter.cpp

endif #ifeq ($(shell uname -m),x86_64)


