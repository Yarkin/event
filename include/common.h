// Copyright (c) 2017-2018 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#pragma once
#ifndef ns1_begin
#define ns1_begin(ns1) \
namespace ns1 {
#endif

#ifndef ns1_end
#define ns1_end() \
}
#endif


#ifndef ns2_begin
#define ns2_begin(ns1, ns2) \
namespace ns1 { \
namespace ns2 {
#endif

#ifndef ns2_end
#define ns2_end() \
} \
}
#endif


#ifndef ns3_begin
#define ns3_begin(ns1, ns2, ns3) \
namespace ns1 { \
namespace ns2 { \
namespace ns3 {
#endif

#ifndef ns3_end
#define ns3_end() \
} \
} \
}
#endif
