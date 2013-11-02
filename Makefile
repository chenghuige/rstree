#COMAKE2 edit-mode: -*- Makefile -*-
####################64Bit Mode####################
ifeq ($(shell uname -m),x86_64)
CC=../../../../../ps/se/toolchain/x86_64-unknown-linux-gnu-4.8.1-2.9-2.20-2.6.32//bin/x86_64-unknown-linux-gnu-g++
CXX=../../../../../ps/se/toolchain/x86_64-unknown-linux-gnu-4.8.1-2.9-2.20-2.6.32//bin/x86_64-unknown-linux-gnu-g++
CXXFLAGS=-g \
  -O \
  -pipe \
  -W \
  -Wall \
  -fPIC \
  -DHAVE_NETINET_IN_H \
  -Wno-unused-parameter \
  -std=c++0x \
  -fpermissive \
  -Wno-write-strings \
  -Wno-literal-suffix \
  -Wno-unused-local-typedefs
CFLAGS=-g \
  -O \
  -pipe \
  -W \
  -Wall \
  -fPIC \
  -DHAVE_NETINET_IN_H \
  -Wno-unused-parameter \
  -std=c++0x \
  -fpermissive \
  -Wno-write-strings \
  -Wno-literal-suffix \
  -Wno-unused-local-typedefs
CPPFLAGS=-D_GNU_SOURCE \
  -D__STDC_LIMIT_MACROS \
  -U_BTEST_ \
  -D__VERSION_ID__=\"1.0.0\" \
  -D__REVISION__=\"$(shell \
  svnversion \
  -n \
  .)\"
INCPATH=-I./include \
  -I./include/post_process \
  -I./utils
DEP_INCPATH=-I../../../../../app/search/sep/anti-spam/rsc-sender \
  -I../../../../../app/search/sep/anti-spam/rsc-sender/include \
  -I../../../../../app/search/sep/anti-spam/rsc-sender/output \
  -I../../../../../app/search/sep/anti-spam/rsc-sender/output/include \
  -I../../../../../com/btest/gtest \
  -I../../../../../com/btest/gtest/include \
  -I../../../../../com/btest/gtest/output \
  -I../../../../../com/btest/gtest/output/include \
  -I../../../../../com/idlcompiler \
  -I../../../../../com/idlcompiler/include \
  -I../../../../../com/idlcompiler/output \
  -I../../../../../com/idlcompiler/output/include \
  -I../../../../../inf/computing/ant \
  -I../../../../../inf/computing/ant/include \
  -I../../../../../inf/computing/ant/output \
  -I../../../../../inf/computing/ant/output/include \
  -I../../../../../inf/computing/java6 \
  -I../../../../../inf/computing/java6/include \
  -I../../../../../inf/computing/java6/output \
  -I../../../../../inf/computing/java6/output/include \
  -I../../../../../inf/computing/zookeeper \
  -I../../../../../inf/computing/zookeeper/include \
  -I../../../../../inf/computing/zookeeper/output \
  -I../../../../../inf/computing/zookeeper/output/include \
  -I../../../../../lib2-64/bsl \
  -I../../../../../lib2-64/bsl/include \
  -I../../../../../lib2-64/bsl/output \
  -I../../../../../lib2-64/bsl/output/include \
  -I../../../../../lib2-64/dict \
  -I../../../../../lib2-64/dict/include \
  -I../../../../../lib2-64/dict/output \
  -I../../../../../lib2-64/dict/output/include \
  -I../../../../../lib2-64/others-ex \
  -I../../../../../lib2-64/others-ex/include \
  -I../../../../../lib2-64/others-ex/output \
  -I../../../../../lib2-64/others-ex/output/include \
  -I../../../../../lib2-64/ullib \
  -I../../../../../lib2-64/ullib/include \
  -I../../../../../lib2-64/ullib/output \
  -I../../../../../lib2-64/ullib/output/include \
  -I../../../../../op/oped/noah/webfoot/naming-lib \
  -I../../../../../op/oped/noah/webfoot/naming-lib/include \
  -I../../../../../op/oped/noah/webfoot/naming-lib/output \
  -I../../../../../op/oped/noah/webfoot/naming-lib/output/include \
  -I../../../../../public/bslext \
  -I../../../../../public/bslext/include \
  -I../../../../../public/bslext/output \
  -I../../../../../public/bslext/output/include \
  -I../../../../../public/comlog-plugin \
  -I../../../../../public/comlog-plugin/include \
  -I../../../../../public/comlog-plugin/output \
  -I../../../../../public/comlog-plugin/output/include \
  -I../../../../../public/configure \
  -I../../../../../public/configure/include \
  -I../../../../../public/configure/output \
  -I../../../../../public/configure/output/include \
  -I../../../../../public/connectpool \
  -I../../../../../public/connectpool/include \
  -I../../../../../public/connectpool/output \
  -I../../../../../public/connectpool/output/include \
  -I../../../../../public/gm/galileo \
  -I../../../../../public/gm/galileo/include \
  -I../../../../../public/gm/galileo/output \
  -I../../../../../public/gm/galileo/output/include \
  -I../../../../../public/idlcompiler \
  -I../../../../../public/idlcompiler/include \
  -I../../../../../public/idlcompiler/output \
  -I../../../../../public/idlcompiler/output/include \
  -I../../../../../public/mcpack \
  -I../../../../../public/mcpack/include \
  -I../../../../../public/mcpack/output \
  -I../../../../../public/mcpack/output/include \
  -I../../../../../public/nshead \
  -I../../../../../public/nshead/include \
  -I../../../../../public/nshead/output \
  -I../../../../../public/nshead/output/include \
  -I../../../../../public/odict \
  -I../../../../../public/odict/include \
  -I../../../../../public/odict/output \
  -I../../../../../public/odict/output/include \
  -I../../../../../public/spreg \
  -I../../../../../public/spreg/include \
  -I../../../../../public/spreg/output \
  -I../../../../../public/spreg/output/include \
  -I../../../../../public/ub \
  -I../../../../../public/ub/include \
  -I../../../../../public/ub/output \
  -I../../../../../public/ub/output/include \
  -I../../../../../public/uconv \
  -I../../../../../public/uconv/include \
  -I../../../../../public/uconv/output \
  -I../../../../../public/uconv/output/include \
  -I../../../../../quality/autotest/reportlib/cpp \
  -I../../../../../quality/autotest/reportlib/cpp/include \
  -I../../../../../quality/autotest/reportlib/cpp/output \
  -I../../../../../quality/autotest/reportlib/cpp/output/include \
  -I../../../../../third-64/boost \
  -I../../../../../third-64/boost/include \
  -I../../../../../third-64/boost/output \
  -I../../../../../third-64/boost/output/include \
  -I../../../../../third-64/gflags \
  -I../../../../../third-64/gflags/include \
  -I../../../../../third-64/gflags/output \
  -I../../../../../third-64/gflags/output/include \
  -I../../../../../third-64/glog \
  -I../../../../../third-64/glog/include \
  -I../../../../../third-64/glog/output \
  -I../../../../../third-64/glog/output/include \
  -I../../../../../third-64/openssl \
  -I../../../../../third-64/openssl/include \
  -I../../../../../third-64/openssl/output \
  -I../../../../../third-64/openssl/output/include \
  -I../../../../../third-64/pcre \
  -I../../../../../third-64/pcre/include \
  -I../../../../../third-64/pcre/output \
  -I../../../../../third-64/pcre/output/include \
  -I../../../../../third-64/protobuf \
  -I../../../../../third-64/protobuf/include \
  -I../../../../../third-64/protobuf/output \
  -I../../../../../third-64/protobuf/output/include \
  -I../../../../../third-64/stlport \
  -I../../../../../third-64/stlport/include \
  -I../../../../../third-64/stlport/output \
  -I../../../../../third-64/stlport/output/include \
  -I../../../../../third-64/tcmalloc \
  -I../../../../../third-64/tcmalloc/include \
  -I../../../../../third-64/tcmalloc/output \
  -I../../../../../third-64/tcmalloc/output/include \
  -I../../../../../third-64/zlib \
  -I../../../../../third-64/zlib/include \
  -I../../../../../third-64/zlib/output \
  -I../../../../../third-64/zlib/output/include

#============ CCP vars ============
CCHECK=@ccheck.py
CCHECK_FLAGS=
PCLINT=@pclint
PCLINT_FLAGS=
CCP=@ccp.py
CCP_FLAGS=


#COMAKE UUID
COMAKE_MD5=371c47298e3829d5421de98c3405564f  COMAKE


.PHONY:all
all:comake2_makefile_check rstree test_filter test_post_processor 
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
	rm -rf rstree
	rm -rf ./output/bin/rstree
	rm -rf test_filter
	rm -rf ./output/bin/test_filter
	rm -rf test_post_processor
	rm -rf ./bin/test_post_processor
	rm -rf src/rstree_conf.o
	rm -rf src/rstree_rstree_main.o
	rm -rf src/rstree_rstree_util.o
	rm -rf src/post_process/rstree_PostProcessor.o
	rm -rf test/test_filter_test_filter.o
	rm -rf src/test_filter_conf.o
	rm -rf src/test_filter_rstree_main.o
	rm -rf src/test_filter_rstree_util.o
	rm -rf src/post_process/test_filter_PostProcessor.o
	rm -rf test/test_post_processor_test_post_processor.o
	rm -rf src/test_post_processor_conf.o
	rm -rf src/test_post_processor_rstree_main.o
	rm -rf src/test_post_processor_rstree_util.o
	rm -rf src/post_process/test_post_processor_PostProcessor.o

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

rstree:src/rstree_conf.o \
  src/rstree_rstree_main.o \
  src/rstree_rstree_util.o \
  src/post_process/rstree_PostProcessor.o
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mrstree[0m']"
	$(CXX) src/rstree_conf.o \
  src/rstree_rstree_main.o \
  src/rstree_rstree_util.o \
  src/post_process/rstree_PostProcessor.o -Xlinker "-("  ../../../../../app/search/sep/anti-spam/rsc-sender/libgezi_util.a \
  ../../../../../com/btest/gtest/output/lib/libgtest.a \
  ../../../../../com/btest/gtest/output/lib/libgtest_main.a \
  ../../../../../com/idlcompiler/astyle/libastyle.a \
  ../../../../../com/idlcompiler/cxx/libskeleton.a \
  ../../../../../com/idlcompiler/java/libjava_skeleton.a \
  ../../../../../com/idlcompiler/parser/libparser.a \
  ../../../../../com/idlcompiler/php/libphp_skeleton.a \
  ../../../../../inf/computing/zookeeper/output/c_client/lib/libzookeeper_mt.a \
  ../../../../../inf/computing/zookeeper/output/c_client/lib/libzookeeper_st.a \
  ../../../../../lib2-64/bsl/lib/libbsl.a \
  ../../../../../lib2-64/bsl/lib/libbsl_ResourcePool.a \
  ../../../../../lib2-64/bsl/lib/libbsl_archive.a \
  ../../../../../lib2-64/bsl/lib/libbsl_buffer.a \
  ../../../../../lib2-64/bsl/lib/libbsl_check_cast.a \
  ../../../../../lib2-64/bsl/lib/libbsl_exception.a \
  ../../../../../lib2-64/bsl/lib/libbsl_pool.a \
  ../../../../../lib2-64/bsl/lib/libbsl_utils.a \
  ../../../../../lib2-64/bsl/lib/libbsl_var.a \
  ../../../../../lib2-64/bsl/lib/libbsl_var_implement.a \
  ../../../../../lib2-64/bsl/lib/libbsl_var_utils.a \
  ../../../../../lib2-64/dict/lib/libuldict.a \
  ../../../../../lib2-64/others-ex/lib/libullib_ex.a \
  ../../../../../lib2-64/ullib/lib/libullib.a \
  ../../../../../op/oped/noah/webfoot/naming-lib/output/lib/libwebfoot_naming.a \
  ../../../../../public/bslext/output/lib/libbsl_bml.a \
  ../../../../../public/bslext/output/lib/libbsl_containers_utils.a \
  ../../../../../public/bslext/output/lib/libbsl_var_scripting.a \
  ../../../../../public/bslext/output/lib/libbsl_var_serializer.a \
  ../../../../../public/bslext/output/lib/libbsl_var_vscript.a \
  ../../../../../public/bslext/output/lib/libbsl_vs.a \
  ../../../../../public/bslext/output/lib/libbslext.a \
  ../../../../../public/comlog-plugin/libcomlog.a \
  ../../../../../public/comlog-plugin/output/lib/libdfsappender.a \
  ../../../../../public/configure/libconfig.a \
  ../../../../../public/connectpool/libconnectpool.a \
  ../../../../../public/gm/galileo/libgalileo.a \
  ../../../../../public/gm/galileo/output/lib/libzookeeper_mt.a \
  ../../../../../public/idlcompiler/output/lib/libmcpack_idl.a \
  ../../../../../public/mcpack/libmcpack.a \
  ../../../../../public/nshead/libnshead.a \
  ../../../../../public/odict/libodict.a \
  ../../../../../public/spreg/libspreg.a \
  ../../../../../public/ub/output/lib/libub.a \
  ../../../../../public/ub/output/lib/libub_aserver.a \
  ../../../../../public/ub/output/lib/libub_client.a \
  ../../../../../public/ub/output/lib/libub_conf.a \
  ../../../../../public/ub/output/lib/libub_galileo.a \
  ../../../../../public/ub/output/lib/libub_log.a \
  ../../../../../public/ub/output/lib/libub_misc.a \
  ../../../../../public/ub/output/lib/libub_monitor.a \
  ../../../../../public/ub/output/lib/libub_naming.a \
  ../../../../../public/ub/output/lib/libub_server.a \
  ../../../../../public/ub/output/lib/libubex.a \
  ../../../../../public/ub/output/lib/libubfw.a \
  ../../../../../public/uconv/libuconv.a \
  ../../../../../quality/autotest/reportlib/cpp/libautotest.a \
  ../../../../../third-64/boost/lib/libboost_atomic.a \
  ../../../../../third-64/boost/lib/libboost_chrono.a \
  ../../../../../third-64/boost/lib/libboost_context.a \
  ../../../../../third-64/boost/lib/libboost_date_time.a \
  ../../../../../third-64/boost/lib/libboost_exception.a \
  ../../../../../third-64/boost/lib/libboost_filesystem.a \
  ../../../../../third-64/boost/lib/libboost_graph.a \
  ../../../../../third-64/boost/lib/libboost_locale.a \
  ../../../../../third-64/boost/lib/libboost_math_c99.a \
  ../../../../../third-64/boost/lib/libboost_math_c99f.a \
  ../../../../../third-64/boost/lib/libboost_math_c99l.a \
  ../../../../../third-64/boost/lib/libboost_math_tr1.a \
  ../../../../../third-64/boost/lib/libboost_math_tr1f.a \
  ../../../../../third-64/boost/lib/libboost_math_tr1l.a \
  ../../../../../third-64/boost/lib/libboost_prg_exec_monitor.a \
  ../../../../../third-64/boost/lib/libboost_program_options.a \
  ../../../../../third-64/boost/lib/libboost_python.a \
  ../../../../../third-64/boost/lib/libboost_random.a \
  ../../../../../third-64/boost/lib/libboost_regex.a \
  ../../../../../third-64/boost/lib/libboost_serialization.a \
  ../../../../../third-64/boost/lib/libboost_signals.a \
  ../../../../../third-64/boost/lib/libboost_system.a \
  ../../../../../third-64/boost/lib/libboost_test_exec_monitor.a \
  ../../../../../third-64/boost/lib/libboost_thread.a \
  ../../../../../third-64/boost/lib/libboost_timer.a \
  ../../../../../third-64/boost/lib/libboost_unit_test_framework.a \
  ../../../../../third-64/boost/lib/libboost_wave.a \
  ../../../../../third-64/boost/lib/libboost_wserialization.a \
  ../../../../../third-64/gflags/lib/libgflags.a \
  ../../../../../third-64/gflags/lib/libgflags_nothreads.a \
  ../../../../../third-64/glog/lib/libglog.a \
  ../../../../../third-64/openssl/lib/libcrypto.a \
  ../../../../../third-64/openssl/lib/libssl.a \
  ../../../../../third-64/pcre/lib/libpcre.a \
  ../../../../../third-64/pcre/lib/libpcrecpp.a \
  ../../../../../third-64/pcre/lib/libpcreposix.a \
  ../../../../../third-64/protobuf/lib/libprotobuf-lite.a \
  ../../../../../third-64/protobuf/lib/libprotobuf.a \
  ../../../../../third-64/protobuf/lib/libprotoc.a \
  ../../../../../third-64/tcmalloc/lib/libprofiler.a \
  ../../../../../third-64/tcmalloc/lib/libtcmalloc.a \
  ../../../../../third-64/tcmalloc/lib/libtcmalloc_and_profiler.a \
  ../../../../../third-64/tcmalloc/lib/libtcmalloc_debug.a \
  ../../../../../third-64/tcmalloc/lib/libtcmalloc_minimal.a \
  ../../../../../third-64/tcmalloc/lib/libtcmalloc_minimal_debug.a \
  ../../../../../third-64/zlib/lib/libz.a -static-libgcc \
  -static-libstdc++ \
  -ldl \
  -lpthread \
  -lrt \
  -rdynamic -Xlinker "-)" -o rstree
	mkdir -p ./output/bin
	cp -f rstree ./output/bin

test_filter:test/test_filter_test_filter.o \
  src/test_filter_conf.o \
  src/test_filter_rstree_main.o \
  src/test_filter_rstree_util.o \
  src/post_process/test_filter_PostProcessor.o
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mtest_filter[0m']"
	$(CXX) test/test_filter_test_filter.o \
  src/test_filter_conf.o \
  src/test_filter_rstree_main.o \
  src/test_filter_rstree_util.o \
  src/post_process/test_filter_PostProcessor.o -Xlinker "-("  ../../../../../app/search/sep/anti-spam/rsc-sender/libgezi_util.a \
  ../../../../../com/btest/gtest/output/lib/libgtest.a \
  ../../../../../com/btest/gtest/output/lib/libgtest_main.a \
  ../../../../../com/idlcompiler/astyle/libastyle.a \
  ../../../../../com/idlcompiler/cxx/libskeleton.a \
  ../../../../../com/idlcompiler/java/libjava_skeleton.a \
  ../../../../../com/idlcompiler/parser/libparser.a \
  ../../../../../com/idlcompiler/php/libphp_skeleton.a \
  ../../../../../inf/computing/zookeeper/output/c_client/lib/libzookeeper_mt.a \
  ../../../../../inf/computing/zookeeper/output/c_client/lib/libzookeeper_st.a \
  ../../../../../lib2-64/bsl/lib/libbsl.a \
  ../../../../../lib2-64/bsl/lib/libbsl_ResourcePool.a \
  ../../../../../lib2-64/bsl/lib/libbsl_archive.a \
  ../../../../../lib2-64/bsl/lib/libbsl_buffer.a \
  ../../../../../lib2-64/bsl/lib/libbsl_check_cast.a \
  ../../../../../lib2-64/bsl/lib/libbsl_exception.a \
  ../../../../../lib2-64/bsl/lib/libbsl_pool.a \
  ../../../../../lib2-64/bsl/lib/libbsl_utils.a \
  ../../../../../lib2-64/bsl/lib/libbsl_var.a \
  ../../../../../lib2-64/bsl/lib/libbsl_var_implement.a \
  ../../../../../lib2-64/bsl/lib/libbsl_var_utils.a \
  ../../../../../lib2-64/dict/lib/libuldict.a \
  ../../../../../lib2-64/others-ex/lib/libullib_ex.a \
  ../../../../../lib2-64/ullib/lib/libullib.a \
  ../../../../../op/oped/noah/webfoot/naming-lib/output/lib/libwebfoot_naming.a \
  ../../../../../public/bslext/output/lib/libbsl_bml.a \
  ../../../../../public/bslext/output/lib/libbsl_containers_utils.a \
  ../../../../../public/bslext/output/lib/libbsl_var_scripting.a \
  ../../../../../public/bslext/output/lib/libbsl_var_serializer.a \
  ../../../../../public/bslext/output/lib/libbsl_var_vscript.a \
  ../../../../../public/bslext/output/lib/libbsl_vs.a \
  ../../../../../public/bslext/output/lib/libbslext.a \
  ../../../../../public/comlog-plugin/libcomlog.a \
  ../../../../../public/comlog-plugin/output/lib/libdfsappender.a \
  ../../../../../public/configure/libconfig.a \
  ../../../../../public/connectpool/libconnectpool.a \
  ../../../../../public/gm/galileo/libgalileo.a \
  ../../../../../public/gm/galileo/output/lib/libzookeeper_mt.a \
  ../../../../../public/idlcompiler/output/lib/libmcpack_idl.a \
  ../../../../../public/mcpack/libmcpack.a \
  ../../../../../public/nshead/libnshead.a \
  ../../../../../public/odict/libodict.a \
  ../../../../../public/spreg/libspreg.a \
  ../../../../../public/ub/output/lib/libub.a \
  ../../../../../public/ub/output/lib/libub_aserver.a \
  ../../../../../public/ub/output/lib/libub_client.a \
  ../../../../../public/ub/output/lib/libub_conf.a \
  ../../../../../public/ub/output/lib/libub_galileo.a \
  ../../../../../public/ub/output/lib/libub_log.a \
  ../../../../../public/ub/output/lib/libub_misc.a \
  ../../../../../public/ub/output/lib/libub_monitor.a \
  ../../../../../public/ub/output/lib/libub_naming.a \
  ../../../../../public/ub/output/lib/libub_server.a \
  ../../../../../public/ub/output/lib/libubex.a \
  ../../../../../public/ub/output/lib/libubfw.a \
  ../../../../../public/uconv/libuconv.a \
  ../../../../../quality/autotest/reportlib/cpp/libautotest.a \
  ../../../../../third-64/boost/lib/libboost_atomic.a \
  ../../../../../third-64/boost/lib/libboost_chrono.a \
  ../../../../../third-64/boost/lib/libboost_context.a \
  ../../../../../third-64/boost/lib/libboost_date_time.a \
  ../../../../../third-64/boost/lib/libboost_exception.a \
  ../../../../../third-64/boost/lib/libboost_filesystem.a \
  ../../../../../third-64/boost/lib/libboost_graph.a \
  ../../../../../third-64/boost/lib/libboost_locale.a \
  ../../../../../third-64/boost/lib/libboost_math_c99.a \
  ../../../../../third-64/boost/lib/libboost_math_c99f.a \
  ../../../../../third-64/boost/lib/libboost_math_c99l.a \
  ../../../../../third-64/boost/lib/libboost_math_tr1.a \
  ../../../../../third-64/boost/lib/libboost_math_tr1f.a \
  ../../../../../third-64/boost/lib/libboost_math_tr1l.a \
  ../../../../../third-64/boost/lib/libboost_prg_exec_monitor.a \
  ../../../../../third-64/boost/lib/libboost_program_options.a \
  ../../../../../third-64/boost/lib/libboost_python.a \
  ../../../../../third-64/boost/lib/libboost_random.a \
  ../../../../../third-64/boost/lib/libboost_regex.a \
  ../../../../../third-64/boost/lib/libboost_serialization.a \
  ../../../../../third-64/boost/lib/libboost_signals.a \
  ../../../../../third-64/boost/lib/libboost_system.a \
  ../../../../../third-64/boost/lib/libboost_test_exec_monitor.a \
  ../../../../../third-64/boost/lib/libboost_thread.a \
  ../../../../../third-64/boost/lib/libboost_timer.a \
  ../../../../../third-64/boost/lib/libboost_unit_test_framework.a \
  ../../../../../third-64/boost/lib/libboost_wave.a \
  ../../../../../third-64/boost/lib/libboost_wserialization.a \
  ../../../../../third-64/gflags/lib/libgflags.a \
  ../../../../../third-64/gflags/lib/libgflags_nothreads.a \
  ../../../../../third-64/glog/lib/libglog.a \
  ../../../../../third-64/openssl/lib/libcrypto.a \
  ../../../../../third-64/openssl/lib/libssl.a \
  ../../../../../third-64/pcre/lib/libpcre.a \
  ../../../../../third-64/pcre/lib/libpcrecpp.a \
  ../../../../../third-64/pcre/lib/libpcreposix.a \
  ../../../../../third-64/protobuf/lib/libprotobuf-lite.a \
  ../../../../../third-64/protobuf/lib/libprotobuf.a \
  ../../../../../third-64/protobuf/lib/libprotoc.a \
  ../../../../../third-64/tcmalloc/lib/libprofiler.a \
  ../../../../../third-64/tcmalloc/lib/libtcmalloc.a \
  ../../../../../third-64/tcmalloc/lib/libtcmalloc_and_profiler.a \
  ../../../../../third-64/tcmalloc/lib/libtcmalloc_debug.a \
  ../../../../../third-64/tcmalloc/lib/libtcmalloc_minimal.a \
  ../../../../../third-64/tcmalloc/lib/libtcmalloc_minimal_debug.a \
  ../../../../../third-64/zlib/lib/libz.a -static-libgcc \
  -static-libstdc++ \
  -ldl \
  -lpthread \
  -lrt \
  -rdynamic -Xlinker "-)" -o test_filter
	mkdir -p ./output/bin
	cp -f test_filter ./output/bin

test_post_processor:test/test_post_processor_test_post_processor.o \
  src/test_post_processor_conf.o \
  src/test_post_processor_rstree_main.o \
  src/test_post_processor_rstree_util.o \
  src/post_process/test_post_processor_PostProcessor.o
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mtest_post_processor[0m']"
	$(CXX) test/test_post_processor_test_post_processor.o \
  src/test_post_processor_conf.o \
  src/test_post_processor_rstree_main.o \
  src/test_post_processor_rstree_util.o \
  src/post_process/test_post_processor_PostProcessor.o -Xlinker "-("  ../../../../../app/search/sep/anti-spam/rsc-sender/libgezi_util.a \
  ../../../../../com/btest/gtest/output/lib/libgtest.a \
  ../../../../../com/btest/gtest/output/lib/libgtest_main.a \
  ../../../../../com/idlcompiler/astyle/libastyle.a \
  ../../../../../com/idlcompiler/cxx/libskeleton.a \
  ../../../../../com/idlcompiler/java/libjava_skeleton.a \
  ../../../../../com/idlcompiler/parser/libparser.a \
  ../../../../../com/idlcompiler/php/libphp_skeleton.a \
  ../../../../../inf/computing/zookeeper/output/c_client/lib/libzookeeper_mt.a \
  ../../../../../inf/computing/zookeeper/output/c_client/lib/libzookeeper_st.a \
  ../../../../../lib2-64/bsl/lib/libbsl.a \
  ../../../../../lib2-64/bsl/lib/libbsl_ResourcePool.a \
  ../../../../../lib2-64/bsl/lib/libbsl_archive.a \
  ../../../../../lib2-64/bsl/lib/libbsl_buffer.a \
  ../../../../../lib2-64/bsl/lib/libbsl_check_cast.a \
  ../../../../../lib2-64/bsl/lib/libbsl_exception.a \
  ../../../../../lib2-64/bsl/lib/libbsl_pool.a \
  ../../../../../lib2-64/bsl/lib/libbsl_utils.a \
  ../../../../../lib2-64/bsl/lib/libbsl_var.a \
  ../../../../../lib2-64/bsl/lib/libbsl_var_implement.a \
  ../../../../../lib2-64/bsl/lib/libbsl_var_utils.a \
  ../../../../../lib2-64/dict/lib/libuldict.a \
  ../../../../../lib2-64/others-ex/lib/libullib_ex.a \
  ../../../../../lib2-64/ullib/lib/libullib.a \
  ../../../../../op/oped/noah/webfoot/naming-lib/output/lib/libwebfoot_naming.a \
  ../../../../../public/bslext/output/lib/libbsl_bml.a \
  ../../../../../public/bslext/output/lib/libbsl_containers_utils.a \
  ../../../../../public/bslext/output/lib/libbsl_var_scripting.a \
  ../../../../../public/bslext/output/lib/libbsl_var_serializer.a \
  ../../../../../public/bslext/output/lib/libbsl_var_vscript.a \
  ../../../../../public/bslext/output/lib/libbsl_vs.a \
  ../../../../../public/bslext/output/lib/libbslext.a \
  ../../../../../public/comlog-plugin/libcomlog.a \
  ../../../../../public/comlog-plugin/output/lib/libdfsappender.a \
  ../../../../../public/configure/libconfig.a \
  ../../../../../public/connectpool/libconnectpool.a \
  ../../../../../public/gm/galileo/libgalileo.a \
  ../../../../../public/gm/galileo/output/lib/libzookeeper_mt.a \
  ../../../../../public/idlcompiler/output/lib/libmcpack_idl.a \
  ../../../../../public/mcpack/libmcpack.a \
  ../../../../../public/nshead/libnshead.a \
  ../../../../../public/odict/libodict.a \
  ../../../../../public/spreg/libspreg.a \
  ../../../../../public/ub/output/lib/libub.a \
  ../../../../../public/ub/output/lib/libub_aserver.a \
  ../../../../../public/ub/output/lib/libub_client.a \
  ../../../../../public/ub/output/lib/libub_conf.a \
  ../../../../../public/ub/output/lib/libub_galileo.a \
  ../../../../../public/ub/output/lib/libub_log.a \
  ../../../../../public/ub/output/lib/libub_misc.a \
  ../../../../../public/ub/output/lib/libub_monitor.a \
  ../../../../../public/ub/output/lib/libub_naming.a \
  ../../../../../public/ub/output/lib/libub_server.a \
  ../../../../../public/ub/output/lib/libubex.a \
  ../../../../../public/ub/output/lib/libubfw.a \
  ../../../../../public/uconv/libuconv.a \
  ../../../../../quality/autotest/reportlib/cpp/libautotest.a \
  ../../../../../third-64/boost/lib/libboost_atomic.a \
  ../../../../../third-64/boost/lib/libboost_chrono.a \
  ../../../../../third-64/boost/lib/libboost_context.a \
  ../../../../../third-64/boost/lib/libboost_date_time.a \
  ../../../../../third-64/boost/lib/libboost_exception.a \
  ../../../../../third-64/boost/lib/libboost_filesystem.a \
  ../../../../../third-64/boost/lib/libboost_graph.a \
  ../../../../../third-64/boost/lib/libboost_locale.a \
  ../../../../../third-64/boost/lib/libboost_math_c99.a \
  ../../../../../third-64/boost/lib/libboost_math_c99f.a \
  ../../../../../third-64/boost/lib/libboost_math_c99l.a \
  ../../../../../third-64/boost/lib/libboost_math_tr1.a \
  ../../../../../third-64/boost/lib/libboost_math_tr1f.a \
  ../../../../../third-64/boost/lib/libboost_math_tr1l.a \
  ../../../../../third-64/boost/lib/libboost_prg_exec_monitor.a \
  ../../../../../third-64/boost/lib/libboost_program_options.a \
  ../../../../../third-64/boost/lib/libboost_python.a \
  ../../../../../third-64/boost/lib/libboost_random.a \
  ../../../../../third-64/boost/lib/libboost_regex.a \
  ../../../../../third-64/boost/lib/libboost_serialization.a \
  ../../../../../third-64/boost/lib/libboost_signals.a \
  ../../../../../third-64/boost/lib/libboost_system.a \
  ../../../../../third-64/boost/lib/libboost_test_exec_monitor.a \
  ../../../../../third-64/boost/lib/libboost_thread.a \
  ../../../../../third-64/boost/lib/libboost_timer.a \
  ../../../../../third-64/boost/lib/libboost_unit_test_framework.a \
  ../../../../../third-64/boost/lib/libboost_wave.a \
  ../../../../../third-64/boost/lib/libboost_wserialization.a \
  ../../../../../third-64/gflags/lib/libgflags.a \
  ../../../../../third-64/gflags/lib/libgflags_nothreads.a \
  ../../../../../third-64/glog/lib/libglog.a \
  ../../../../../third-64/openssl/lib/libcrypto.a \
  ../../../../../third-64/openssl/lib/libssl.a \
  ../../../../../third-64/pcre/lib/libpcre.a \
  ../../../../../third-64/pcre/lib/libpcrecpp.a \
  ../../../../../third-64/pcre/lib/libpcreposix.a \
  ../../../../../third-64/protobuf/lib/libprotobuf-lite.a \
  ../../../../../third-64/protobuf/lib/libprotobuf.a \
  ../../../../../third-64/protobuf/lib/libprotoc.a \
  ../../../../../third-64/tcmalloc/lib/libprofiler.a \
  ../../../../../third-64/tcmalloc/lib/libtcmalloc.a \
  ../../../../../third-64/tcmalloc/lib/libtcmalloc_and_profiler.a \
  ../../../../../third-64/tcmalloc/lib/libtcmalloc_debug.a \
  ../../../../../third-64/tcmalloc/lib/libtcmalloc_minimal.a \
  ../../../../../third-64/tcmalloc/lib/libtcmalloc_minimal_debug.a \
  ../../../../../third-64/zlib/lib/libz.a -static-libgcc \
  -static-libstdc++ \
  -ldl \
  -lpthread \
  -lrt \
  -rdynamic -Xlinker "-)" -o test_post_processor
	mkdir -p ./bin
	cp -f test_post_processor ./bin

src/rstree_conf.o:src/conf.cpp \
  include/conf.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/rstree_conf.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/rstree_conf.o src/conf.cpp

src/rstree_rstree_main.o:src/rstree_main.cpp \
  include/rstree_def.h \
  include/rstree_util.h \
  include/DSuffixTree.h \
  include/SuffixTree.h \
  include/rstree_def.h \
  include/post_process/RstreeFilter.h \
  include/post_process/PostProcessor.h \
  include/post_process/RstreeFilter.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/rstree_rstree_main.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/rstree_rstree_main.o src/rstree_main.cpp

src/rstree_rstree_util.o:src/rstree_util.cpp \
  include/rstree_util.h \
  include/rstree_def.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/rstree_rstree_util.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/rstree_rstree_util.o src/rstree_util.cpp

src/post_process/rstree_PostProcessor.o:src/post_process/PostProcessor.cpp \
  include/post_process/PostProcessor.h \
  include/post_process/RstreeFilter.h \
  include/DSuffixTree.h \
  include/SuffixTree.h \
  include/rstree_def.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/post_process/rstree_PostProcessor.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/post_process/rstree_PostProcessor.o src/post_process/PostProcessor.cpp

test/test_filter_test_filter.o:test/test_filter.cc \
  include/post_process/RstreeFilter.h \
  include/DSuffixTree.h \
  include/SuffixTree.h \
  include/rstree_def.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mtest/test_filter_test_filter.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o test/test_filter_test_filter.o test/test_filter.cc

src/test_filter_conf.o:src/conf.cpp \
  include/conf.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/test_filter_conf.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/test_filter_conf.o src/conf.cpp

src/test_filter_rstree_main.o:src/rstree_main.cpp \
  include/rstree_def.h \
  include/rstree_util.h \
  include/DSuffixTree.h \
  include/SuffixTree.h \
  include/rstree_def.h \
  include/post_process/RstreeFilter.h \
  include/post_process/PostProcessor.h \
  include/post_process/RstreeFilter.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/test_filter_rstree_main.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/test_filter_rstree_main.o src/rstree_main.cpp

src/test_filter_rstree_util.o:src/rstree_util.cpp \
  include/rstree_util.h \
  include/rstree_def.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/test_filter_rstree_util.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/test_filter_rstree_util.o src/rstree_util.cpp

src/post_process/test_filter_PostProcessor.o:src/post_process/PostProcessor.cpp \
  include/post_process/PostProcessor.h \
  include/post_process/RstreeFilter.h \
  include/DSuffixTree.h \
  include/SuffixTree.h \
  include/rstree_def.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/post_process/test_filter_PostProcessor.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/post_process/test_filter_PostProcessor.o src/post_process/PostProcessor.cpp

test/test_post_processor_test_post_processor.o:test/test_post_processor.cc \
  include/post_process/PostProcessor.h \
  include/post_process/RstreeFilter.h \
  include/DSuffixTree.h \
  include/SuffixTree.h \
  include/rstree_def.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mtest/test_post_processor_test_post_processor.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o test/test_post_processor_test_post_processor.o test/test_post_processor.cc

src/test_post_processor_conf.o:src/conf.cpp \
  include/conf.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/test_post_processor_conf.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/test_post_processor_conf.o src/conf.cpp

src/test_post_processor_rstree_main.o:src/rstree_main.cpp \
  include/rstree_def.h \
  include/rstree_util.h \
  include/DSuffixTree.h \
  include/SuffixTree.h \
  include/rstree_def.h \
  include/post_process/RstreeFilter.h \
  include/post_process/PostProcessor.h \
  include/post_process/RstreeFilter.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/test_post_processor_rstree_main.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/test_post_processor_rstree_main.o src/rstree_main.cpp

src/test_post_processor_rstree_util.o:src/rstree_util.cpp \
  include/rstree_util.h \
  include/rstree_def.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/test_post_processor_rstree_util.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/test_post_processor_rstree_util.o src/rstree_util.cpp

src/post_process/test_post_processor_PostProcessor.o:src/post_process/PostProcessor.cpp \
  include/post_process/PostProcessor.h \
  include/post_process/RstreeFilter.h \
  include/DSuffixTree.h \
  include/SuffixTree.h \
  include/rstree_def.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/post_process/test_post_processor_PostProcessor.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/post_process/test_post_processor_PostProcessor.o src/post_process/PostProcessor.cpp

endif #ifeq ($(shell uname -m),x86_64)


