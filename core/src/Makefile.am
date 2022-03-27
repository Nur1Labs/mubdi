# Copyright (c) 2013-2016 The Bitcoin Core developers
# Copyright (c) 2010-2022 Nur1Labs
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

DIST_SUBDIRS = secp256k1 univalue

AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS) $(HARDENED_LDFLAGS) $(GPROF_LDFLAGS) $(SANITIZER_LDFLAGS)
AM_CXXFLAGS = $(DEBUG_CXXFLAGS) $(HARDENED_CXXFLAGS) $(WARN_CXXFLAGS) $(NOWARN_CXXFLAGS) $(ERROR_CXXFLAGS) $(GPROF_CXXFLAGS) $(SANITIZER_CXXFLAGS)
AM_CPPFLAGS = $(DEBUG_CPPFLAGS) $(HARDENED_CPPFLAGS)
AM_LIBTOOLFLAGS = --preserve-dup-deps
EXTRA_LIBRARIES =

if EMBEDDED_UNIVALUE
LIBUNIVALUE = univalue/libunivalue.la

$(LIBUNIVALUE): $(wildcard univalue/lib/*) $(wildcard univalue/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)
else
LIBUNIVALUE = $(UNIVALUE_LIBS)
endif

MUBDI_CONFIG_INCLUDES=-I$(builddir)/config
MUBDI_INCLUDES=-I$(builddir) -I$(builddir)/obj $(BDB_CPPFLAGS) $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS)

MUBDI_INCLUDES += -I$(srcdir)/secp256k1/include
MUBDI_INCLUDES += $(UNIVALUE_CFLAGS)

LIBMUBDI_SERVER=libmubdi_server.a
LIBMUBDI_COMMON=libmubdi_common.a
LIBMUBDI_CLI=libmubdi_cli.a
LIBMUBDI_UTIL=libmubdi_util.a
LIBMUBDI_CRYPTO=crypto/libmubdi_crypto.a
LIBMUBDI_AIRLOGIC=libairlogic/libmubdi_airlogic.a
LIBMUBDIQT=qt/libmubdiqt.a
LIBSECP256K1=secp256k1/libsecp256k1.la

if ENABLE_ZMQ
LIBMUBDI_ZMQ=libmubdi_zmq.a
endif
if BUILD_MUBDI_LIBS
LIBMUBDICONSENSUS=libmubdiconsensus.la
endif
if ENABLE_WALLET
LIBMUBDI_WALLET=libmubdi_wallet.a
endif

$(LIBSECP256K1): $(wildcard secp256k1/src/*.h) $(wildcard secp256k1/src/*.c) $(wildcard secp256k1/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

# Make is not made aware of per-object dependencies to avoid limiting building parallelization
# But to build the less dependent modules first, we manually select their order here:
EXTRA_LIBRARIES += \
  $(LIBMUBDI_CRYPTO) \
  $(LIBMUBDI_UTIL) \
  $(LIBMUBDI_COMMON) \
  $(LIBMUBDI_AIRLOGIC) \
  $(LIBMUBDI_SERVER) \
  $(LIBMUBDI_CLI) \
  $(LIBMUBDI_WALLET) \
  $(LIBMUBDI_ZMQ)

lib_LTLIBRARIES = $(LIBMUBDICONSENSUS)

bin_PROGRAMS =
noinst_PROGRAMS =
TESTS =
BENCHMARKS =

if BUILD_MUBDID
  bin_PROGRAMS += cariad
endif

if BUILD_MUBDI_UTILS
  bin_PROGRAMS += caria-cli caria-tx
endif

.PHONY: FORCE check-symbols check-security
# mubdi core #
MUBDI_CORE_H = \
  activemasternode.h \
  addrman.h \
  alert.h \
  allocators.h \
  amount.h \
  base58.h \
  bip38.h \
  bloom.h \
  blocksignature.h \
  chain.h \
  chainparams.h \
  chainparamsbase.h \
  chainparamsseeds.h \
  checkpoints.h \
  checkqueue.h \
  clientversion.h \
  coincontrol.h \
  coins.h \
  compat.h \
  compat/byteswap.h \
  compat/endian.h \
  compat/sanity.h \
  compressor.h \
  consensus/consensus.h \
  consensus/merkle.h \
  consensus/validation.h \
  consensus/tx_verify.h \
  consensus/params.h \
  primitives/block.h \
  primitives/transaction.h \
  core_io.h \
  crypter.h \
  pairresult.h \
  addressbook.h \
  denomination_functions.h \
  obfuscation.h \
  obfuscation-relay.h \
  wallet/db.h \
  hash.h \
  httprpc.h \
  httpserver.h \
  init.h \
  kernel.h \
  swifttx.h \
  key.h \
  keystore.h \
  leveldbwrapper.h \
  limitedmap.h \
  main.h \
  masternode.h \
  masternode-payments.h \
  masternode-budget.h \
  masternode-sync.h \
  masternodeman.h \
  masternodeconfig.h \
  merkleblock.h \
  messagesigner.h \
  miner.h \
  mruset.h \
  netbase.h \
  net.h \
  noui.h \
  pow.h \
  protocol.h \
  pubkey.h \
  random.h \
  reverselock.h \
  reverse_iterate.h \
  rpc/client.h \
  rpc/protocol.h \
  rpc/server.h \
  scheduler.h \
  script/interpreter.h \
  script/script.h \
  script/sigcache.h \
  script/sign.h \
  script/standard.h \
  script/script_error.h \
  serialize.h \
  spork.h \
  sporkdb.h \
  sporkid.h \
  stakeinput.h \
  streams.h \
  support/cleanse.h \
  sync.h \
  threadsafety.h \
  timedata.h \
  tinyformat.h \
  torcontrol.h \
  txdb.h \
  txmempool.h \
  guiinterface.h \
  uint256.h \
  undo.h \
  util.h \
  util/macros.h \
  util/threadnames.h \
  utilstrencodings.h \
  utilmoneystr.h \
  utiltime.h \
  validationinterface.h \
  version.h \
  wallet/wallet.h \
  wallet/wallet_ismine.h \
  wallet/walletdb.h \
  zmub/zerocoin.h \
  zmq/zmqabstractnotifier.h \
  zmq/zmqconfig.h \
  zmq/zmqnotificationinterface.h \
  zmq/zmqpublishnotifier.h

obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	@$(top_srcdir)/share/genbuild.sh "$(abs_top_builddir)/src/obj/build.h" \
	  "$(abs_top_srcdir)"
libmubdi_util_a-clientversion.$(OBJEXT): obj/build.h

# server: shared between mubdid and mubdi-qt
libmubdi_server_a_CPPFLAGS = $(AM_CPPFLAGS) $(MUBDI_INCLUDES) $(MINIUPNPC_CPPFLAGS) $(EVENT_CFLAGS) $(EVENT_PTHREADS_CFLAGS)
libmubdi_server_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libmubdi_server_a_SOURCES = \
  addrman.cpp \
  alert.cpp \
  bloom.cpp \
  blocksignature.cpp \
  chain.cpp \
  checkpoints.cpp \
  consensus/tx_verify.cpp \
  httprpc.cpp \
  httpserver.cpp \
  init.cpp \
  leveldbwrapper.cpp \
  main.cpp \
  merkleblock.cpp \
  miner.cpp \
  net.cpp \
  noui.cpp \
  pow.cpp \
  rest.cpp \
  rpc/blockchain.cpp \
  rpc/masternode.cpp \
  rpc/budget.cpp \
  rpc/mining.cpp \
  rpc/misc.cpp \
  rpc/net.cpp \
  rpc/rawtransaction.cpp \
  rpc/server.cpp \
  script/sigcache.cpp \
  sporkdb.cpp \
  timedata.cpp \
  torcontrol.cpp \
  txdb.cpp \
  txmempool.cpp \
  validationinterface.cpp \
  $(MUBDI_CORE_H)

if ENABLE_ZMQ
libmubdi_zmq_a_CPPFLAGS = $(MUBDI_INCLUDES) $(ZMQ_CFLAGS)
libmubdi_zmq_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libmubdi_zmq_a_SOURCES = \
  zmq/zmqabstractnotifier.cpp \
  zmq/zmqnotificationinterface.cpp \
  zmq/zmqpublishnotifier.cpp
endif

# wallet: shared between mubdid and mubdi-qt, but only linked
# when wallet enabled
libmubdi_wallet_a_CPPFLAGS = $(AM_CPPFLAGS) $(MUBDI_INCLUDES)
libmubdi_wallet_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libmubdi_wallet_a_SOURCES = \
  activemasternode.cpp \
  bip38.cpp \
  denomination_functions.cpp \
  obfuscation.cpp \
  obfuscation-relay.cpp \
  wallet/db.cpp \
  addressbook.cpp \
  crypter.cpp \
  swifttx.cpp \
  masternode.cpp \
  masternode-budget.cpp \
  masternode-payments.cpp \
  masternode-sync.cpp \
  masternodeconfig.cpp \
  masternodeman.cpp \
  messagesigner.cpp \
  wallet/rpcdump.cpp \
  wallet/rpcwallet.cpp \
  kernel.cpp \
  wallet/wallet.cpp \
  wallet/wallet_ismine.cpp \
  wallet/walletdb.cpp \
  zmub/zerocoin.cpp \
  stakeinput.cpp \
  $(MUBDI_CORE_H)

# crypto primitives library
crypto_libmubdi_crypto_a_CPPFLAGS = $(AM_CPPFLAGS) $(PIC_FLAGS)
crypto_libmubdi_crypto_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIC_FLAGS)
crypto_libmubdi_crypto_a_SOURCES = \
  crypto/sha1.cpp \
  crypto/sha256.cpp \
  crypto/sha512.cpp \
  crypto/chacha20.h \
  crypto/chacha20.cpp \
  crypto/hmac_sha256.cpp \
  crypto/rfc6979_hmac_sha256.cpp \
  crypto/hmac_sha512.cpp \
  crypto/scrypt.cpp \
  crypto/ripemd160.cpp \
  crypto/sph_md_helper.c \
  crypto/sph_sha2big.c \
  crypto/aes_helper.c \
  crypto/blake.c \
  crypto/bmw.c \
  crypto/groestl.c \
  crypto/jh.c \
  crypto/luffa.c \
  crypto/keccak.c \
  crypto/skein.c \
  crypto/cubehash.c \
  crypto/shavite.c \
  crypto/shabal.c \
  crypto/simd.c \
  crypto/echo.c \
  crypto/whirlpool.c \
  crypto/haval.c \
  crypto/hamsi.c \
  crypto/fugue.c \
  crypto/sha2.c \
  crypto/common.h \
  crypto/sha256.h \
  crypto/sha512.h \
  crypto/hmac_sha256.h \
  crypto/rfc6979_hmac_sha256.h \
  crypto/hmac_sha512.h \
  crypto/scrypt.h \
  crypto/sha1.h \
  crypto/ripemd160.h \
  crypto/sph_blake.h \
  crypto/sph_bmw.h \
  crypto/sph_groestl.h \
  crypto/sph_jh.h \
  crypto/sph_keccak.h \
  crypto/sph_luffa.h \
  crypto/sph_shavite.h \
  crypto/sph_hamsi.h \
  crypto/sph_cubehash.h \
  crypto/sph_skein.h \
  crypto/sph_simd.h \
  crypto/sph_fugue.h \
  crypto/sph_echo.h \
  crypto/sph_shabal.h \
  crypto/sph_whirlpool.h \
  crypto/sph_sha2.h \
  crypto/sph_haval.h \
  crypto/sph_types.h

# libairlogic library
libairlogic_libmubdi_airlogic_a_CPPFLAGS = $(AM_CPPFLAGS) $(BOOST_CPPFLAGS)
libairlogic_libmubdi_airlogic_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libairlogic_libmubdi_airlogic_a_SOURCES = \
  libairlogic/Accumulator.h \
  libairlogic/AccumulatorProofOfKnowledge.h \
  libairlogic/bignum.h \
  libairlogic/Coin.h \
  libairlogic/CoinSpend.h \
  libairlogic/Commitment.h \
  libairlogic/Denominations.h \
  libairlogic/ParamGeneration.h \
  libairlogic/Params.h \
  libairlogic/SerialNumberSignatureOfKnowledge.h \
  libairlogic/SpendType.h \
  libairlogic/ZerocoinDefines.h \
  libairlogic/Accumulator.cpp \
  libairlogic/AccumulatorProofOfKnowledge.cpp \
  libairlogic/Coin.cpp \
  libairlogic/Denominations.cpp \
  libairlogic/CoinSpend.cpp \
  libairlogic/Commitment.cpp \
  libairlogic/ParamGeneration.cpp \
  libairlogic/Params.cpp \
  libairlogic/SerialNumberSignatureOfKnowledge.cpp

# common: shared between mubdid, and mubdi-qt and non-server tools
libmubdi_common_a_CPPFLAGS = $(AM_CPPFLAGS) $(MUBDI_INCLUDES)
libmubdi_common_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libmubdi_common_a_SOURCES = \
  allocators.cpp \
  amount.cpp \
  base58.cpp \
  bip38.cpp \
  chainparams.cpp \
  coins.cpp \
  compressor.cpp \
  consensus/merkle.cpp \
  primitives/block.cpp \
  primitives/transaction.cpp \
  zmub/zerocoin.cpp \
  core_read.cpp \
  core_write.cpp \
  hash.cpp \
  key.cpp \
  keystore.cpp \
  netbase.cpp \
  protocol.cpp \
  pubkey.cpp \
  scheduler.cpp \
  script/interpreter.cpp \
  script/script.cpp \
  script/sign.cpp \
  script/standard.cpp \
  script/script_error.cpp \
  spork.cpp \
  sporkdb.cpp \
  $(MUBDI_CORE_H)

# util: shared between all executables.
# This library *must* be included to make sure that the glibc
# backward-compatibility objects and their sanity checks are linked.
libmubdi_util_a_CPPFLAGS = $(AM_CPPFLAGS) $(MUBDI_INCLUDES)
libmubdi_util_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libmubdi_util_a_SOURCES = \
  allocators.cpp \
  chainparamsbase.cpp \
  clientversion.cpp \
  compat/glibc_sanity.cpp \
  compat/glibcxx_sanity.cpp \
  compat/strnlen.cpp \
  random.cpp \
  rpc/protocol.cpp \
  support/cleanse.cpp \
  sync.cpp \
  uint256.cpp \
  util.cpp \
  utilmoneystr.cpp \
  util/threadnames.cpp \
  utilstrencodings.cpp \
  utiltime.cpp \
  $(MUBDI_CORE_H)

if GLIBC_BACK_COMPAT
libmubdi_util_a_SOURCES += compat/glibc_compat.cpp
AM_LDFLAGS += $(COMPAT_LDFLAGS)
endif

# cli: shared between mubdi-cli and mubdi-qt
libmubdi_cli_a_CPPFLAGS = $(AM_CPPFLAGS) $(MUBDI_INCLUDES)
libmubdi_cli_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libmubdi_cli_a_SOURCES = \
  rpc/client.cpp \
  $(MUBDI_CORE_H)

nodist_libmubdi_util_a_SOURCES = $(srcdir)/obj/build.h
#

# mubdid binary #
cariad_SOURCES = mubdid.cpp
cariad_CPPFLAGS = $(AM_CPPFLAGS) $(MUBDI_INCLUDES)
cariad_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
cariad_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
cariad_SOURCES += mubdid-res.rc
endif
cariad_LDADD = \
  $(LIBMUBDI_SERVER) \
  $(LIBMUBDI_WALLET) \
  $(LIBMUBDI_COMMON) \
  $(LIBUNIVALUE) \
  $(LIBMUBDI_AIRLOGIC) \
  $(LIBMUBDI_UTIL) \
  $(LIBMUBDI_ZMQ) \
  $(LIBMUBDI_CRYPTO) \
  $(LIBLEVELDB) \
  $(LIBLEVELDB_SSE42) \
  $(LIBMEMENV) \
  $(LIBSECP256K1)

cariad_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(MINIUPNPC_LIBS) $(EVENT_PTHREADS_LIBS) $(EVENT_LIBS) $(ZMQ_LIBS)

# mubdi-cli binary #
caria_cli_SOURCES = mubdi-cli.cpp
caria_cli_CPPFLAGS = $(AM_CPPFLAGS) $(MUBDI_INCLUDES) $(EVENT_CFLAGS)
caria_cli_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
caria_cli_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
caria_cli_SOURCES += mubdi-cli-res.rc
endif

caria_cli_LDADD = \
  $(LIBMUBDI_CLI) \
  $(LIBUNIVALUE) \
  $(LIBMUBDI_UTIL) \
  $(LIBMUBDI_CRYPTO)

caria_cli_LDADD += $(BOOST_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(EVENT_LIBS)
#

# caria-tx binary #
caria_tx_SOURCES = mubdi-tx.cpp
caria_tx_CPPFLAGS = $(AM_CPPFLAGS) $(MUBDI_INCLUDES)
caria_tx_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
caria_tx_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
caria_tx_SOURCES += mubdi-tx-res.rc
endif

caria_tx_LDADD = \
  $(LIBUNIVALUE) \
  $(LIBMUBDI_COMMON) \
  $(LIBMUBDI_AIRLOGIC) \
  $(LIBMUBDI_UTIL) \
  $(LIBMUBDI_CRYPTO) \
  $(LIBSECP256K1)

caria_tx_LDADD += $(BOOST_LIBS) $(CRYPTO_LIBS)
#

# mubdiconsensus library #
if BUILD_MUBDI_LIBS
include_HEADERS = script/mubdiconsensus.h
libmubdiconsensus_la_SOURCES = \
  allocators.cpp \
  primitives/transaction.cpp \
  crypto/hmac_sha512.cpp \
  crypto/scrypt.cpp \
  crypto/sha1.cpp \
  crypto/sha256.cpp \
  crypto/sha512.cpp \
  crypto/ripemd160.cpp \
  hash.cpp \
  pubkey.cpp \
  script/script.cpp \
  script/interpreter.cpp \
  script/mubdiconsensus.cpp \
  uint256.cpp \
  utilstrencodings.cpp

if GLIBC_BACK_COMPAT
  libmubdiconsensus_la_SOURCES += compat/glibc_compat.cpp
endif

libmubdiconsensus_la_LDFLAGS = $(AM_LDFLAGS) -no-undefined $(RELDFLAGS)
libmubdiconsensus_la_LIBADD = $(LIBSECP256K1)
libmubdiconsensus_la_CPPFLAGS = $(AM_CPPFLAGS) -I$(builddir)/obj -I$(srcdir)/secp256k1/include -DBUILD_MUBDI_INTERNAL
libmubdiconsensus_la_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)

endif
#

CLEANFILES = $(EXTRA_LIBRARIES)

CLEANFILES += *.gcda *.gcno
CLEANFILES += compat/*.gcda compat/*.gcno
CLEANFILES += consensus/*.gcda consensus/*.gcno
CLEANFILES += crypto/*.gcda crypto/*.gcno
CLEANFILES += libairlogic/*.gcda libairlogic/*.gcno
CLEANFILES += primitives/*.gcda primitives/*.gcno
CLEANFILES += rpc/*.gcda rpc/*.gcno
CLEANFILES += script/*.gcda script/*.gcno
CLEANFILES += support/*.gcda support/*.gcno
CLEANFILES += univalue/*.gcda univalue/*.gcno
CLEANFILES += wallet/*.gcda wallet/*.gcno
CLEANFILES += wallet/test/*.gcda wallet/test/*.gcno
CLEANFILES += zmq/*.gcda zmq/*.gcno
CLEANFILES += zmub/*.gcda zmub/*.gcno
CLEANFILES += obj/build.h

EXTRA_DIST =


config/mubdi-config.h: config/stamp-h1
	@$(MAKE) -C $(top_builddir) $(subdir)/$(@)
config/stamp-h1: $(top_srcdir)/$(subdir)/config/mubdi-config.h.in $(top_builddir)/config.status
	$(AM_V_at)$(MAKE) -C $(top_builddir) $(subdir)/$(@)
$(top_srcdir)/$(subdir)/config/mubdi-config.h.in:  $(am__configure_deps)
	$(AM_V_at)$(MAKE) -C $(top_srcdir) $(subdir)/config/mubdi-config.h.in

clean-local:
	-$(MAKE) -C secp256k1 clean
	-$(MAKE) -C univalue clean
	-rm -f leveldb/*/*.gcda leveldb/*/*.gcno leveldb/helpers/memenv/*.gcda leveldb/helpers/memenv/*.gcno
	-rm -f config.h
	-rm -rf test/__pycache__

.rc.o:
	@test -f $(WINDRES)
	## FIXME: How to get the appropriate modulename_CPPFLAGS in here?
	$(AM_V_GEN) $(WINDRES) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(CPPFLAGS) -DWINDRES_PREPROC -i $< -o $@

check-symbols: $(bin_PROGRAMS)
if GLIBC_BACK_COMPAT
	@echo "Checking glibc back compat..."
	$(AM_V_at) READELF=$(READELF) CPPFILT=$(CPPFILT) $(PYTHON) $(top_srcdir)/contrib/devtools/symbol-check.py < $(bin_PROGRAMS)
endif

check-security: $(bin_PROGRAMS)
if HARDEN
	@echo "Checking binary security..."
	$(AM_V_at) READELF=$(READELF) OBJDUMP=$(OBJDUMP) $(PYTHON) $(top_srcdir)/contrib/devtools/security-check.py < $(bin_PROGRAMS)
endif

%.pb.cc %.pb.h: %.proto
	@test -f $(PROTOC)
	$(AM_V_GEN) $(PROTOC) --cpp_out=$(@D) --proto_path=$(<D) $<

if EMBEDDED_LEVELDB
include Makefile.leveldb.include
endif

if ENABLE_TESTS
include Makefile.test.include
endif

if ENABLE_QT
include Makefile.qt.include
endif

if ENABLE_QT_TESTS
include Makefile.qttest.include
endif
