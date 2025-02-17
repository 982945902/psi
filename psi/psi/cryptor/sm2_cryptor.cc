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

#include "psi/psi/cryptor/sm2_cryptor.h"

#include "absl/types/span.h"
#include "openssl/bn.h"
#include "openssl/ec.h"
#include "yacl/crypto/base/hash/hash_utils.h"
#include "yacl/utils/parallel.h"

#include "psi/psi/cryptor/ecc_utils.h"

namespace psi::psi {

void Sm2Cryptor::EccMask(absl::Span<const char> batch_points,
                         absl::Span<char> dest_points) const {
  YACL_ENFORCE(batch_points.size() % kEcPointCompressLength == 0, "{} % {}!=0",
               batch_points.size(), kEcPointCompressLength);

  using Item = std::array<char, kEcPointCompressLength>;
  static_assert(sizeof(Item) == kEcPointCompressLength);

  auto mask_functor = [this](const Item& in, Item& out) {
    BnCtxPtr bn_ctx(yacl::CheckNotNull(BN_CTX_new()));

    EcGroupSt ec_group(GetEcGroupId(curve_type_));

    EcPointSt ec_point(ec_group);

    EC_POINT_oct2point(ec_group.get(), ec_point.get(),
                       reinterpret_cast<const unsigned char*>(in.data()),
                       in.size(), bn_ctx.get());

    BigNumSt bn_sk;
    bn_sk.FromBytes(
        absl::string_view(reinterpret_cast<const char*>(&this->private_key_[0]),
                          kEccKeySize),
        ec_group.bn_p);

    // pointmul

    EcPointSt ec_point2 = ec_point.PointMul(ec_group, bn_sk);

    ec_point2.ToBytes(
        absl::MakeSpan(reinterpret_cast<uint8_t*>(out.data()), out.size()));
  };

  absl::Span<const Item> input(
      reinterpret_cast<const Item*>(batch_points.data()),
      batch_points.size() / sizeof(Item));
  absl::Span<Item> output(reinterpret_cast<Item*>(dest_points.data()),
                          dest_points.size() / sizeof(Item));

  yacl::parallel_for(0, input.size(), [&](int64_t begin, int64_t end) {
    for (int64_t idx = begin; idx < end; ++idx) {
      mask_functor(input[idx], output[idx]);
    }
  });
}

size_t Sm2Cryptor::GetMaskLength() const { return kEcPointCompressLength; }

std::vector<uint8_t> Sm2Cryptor::HashToCurve(
    absl::Span<const char> item_data) const {
  EcGroupSt ec_group(GetEcGroupId(curve_type_));

  EcPointSt ec_point = EcPointSt::CreateEcPointByHashToCurve(
      absl::string_view(item_data.data(), item_data.size()), ec_group);

  std::vector<uint8_t> out(kEcPointCompressLength, 0);

  ec_point.ToBytes(absl::MakeSpan(out.data(), out.size()));

  return out;
}

}  // namespace psi::psi
