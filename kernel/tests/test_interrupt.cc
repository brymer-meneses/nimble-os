#include "prelude.h"
#include "../interrupt/interrupt.h"

TEST(interrupt, trivial_assertion) {
  ASSERT_EQ(1,1);
}

TEST(interrupt, another_test) {
  ASSERT_EQ(1,2);
}
