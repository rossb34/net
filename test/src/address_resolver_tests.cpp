
#include <net/address_resolver.hpp>
#include "gtest/gtest.h"

namespace {

    TEST(AddressResolverTests, ResolveValidAddress) {
        net::AddressResolver resolver;
        auto *addressInfo = resolver.resolve("localhost", "40000");
        EXPECT_TRUE(addressInfo);
    }

    TEST(AddressResolverTests, ResolveInvalidAddress) {
        net::AddressResolver resolver;
        EXPECT_ANY_THROW(resolver.resolve("foo", "bar"));
    }

}
