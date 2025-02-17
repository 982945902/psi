// Copyright 2022 Ant Group Co., Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "psi/psi/cryptor/cryptor_selector.h"

#include <cstdlib>

#include "spdlog/spdlog.h"
#include "yacl/utils/platform_utils.h"

#include "psi/psi/cryptor/fourq_cryptor.h"
#include "psi/psi/cryptor/sm2_cryptor.h"
#include "psi/psi/cryptor/sodium_curve25519_cryptor.h"
#include "psi/psi/prelude.h"

#ifdef __x86_64__
#include "psi/psi/cryptor/ipp_ecc_cryptor.h"
#endif

namespace psi::psi {

namespace {

std::unique_ptr<IEccCryptor> GetIppCryptor() {
#ifdef __x86_64__
  if (yacl::hasAVX512ifma()) {
    SPDLOG_INFO("Using IPPCP");
    return std::make_unique<IppEccCryptor>();
  }
#endif
  return {};
}

std::unique_ptr<IEccCryptor> GetSodiumCryptor() {
  SPDLOG_INFO("Using libSodium");
  return std::make_unique<SodiumCurve25519Cryptor>();
}

std::unique_ptr<IEccCryptor> GetFourQCryptor() {
#ifdef __x86_64__
  if (yacl::hasAVX2()) {
#endif
    SPDLOG_INFO("Using FourQ");
    return std::make_unique<FourQEccCryptor>();  // fourq has an arm impl,
                                                 // so always works on ARM
                                                 // platform
#ifdef __x86_64__
  }
#endif
  return {};
}
}  // namespace

std::unique_ptr<IEccCryptor> CreateEccCryptor(CurveType type) {
  std::unique_ptr<IEccCryptor> cryptor;
  switch (type) {
    case CurveType::CURVE_25519: {
      cryptor = GetIppCryptor();
      if (cryptor == nullptr) {
        cryptor = GetSodiumCryptor();
      }
      break;
    }
    case CurveType::CURVE_FOURQ: {
      cryptor = GetFourQCryptor();
      YACL_ENFORCE(cryptor != nullptr, "FourQ requires AVX2 instruction");
      break;
    }
    case CurveType::CURVE_SM2: {
      SPDLOG_INFO("Using SM2");
      cryptor = std::make_unique<Sm2Cryptor>(type);
      break;
    }
    case CurveType::CURVE_SECP256K1: {
      SPDLOG_INFO("Using Secp256k1");
      cryptor = std::make_unique<Sm2Cryptor>(type);
      break;
    }
    default: {
      YACL_THROW("Invaild curve type: {}", type);
    }
  }
  YACL_ENFORCE(cryptor != nullptr, "Cryptor should not be nullptr");
  return cryptor;
}

}  // namespace psi::psi
