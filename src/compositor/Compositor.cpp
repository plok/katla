//
// Created by jorrit on 6/11/18.
//

#include "Compositor.h"
#include <string>

#include <kms++/kms++.h>
#include <kms++util/kms++util.h>
#include <iostream>

#include "rxcpp/rx.hpp"
namespace Rx {
    using namespace rxcpp;
    using namespace rxcpp::sources;
    using namespace rxcpp::operators;
    using namespace rxcpp::util;
}
using namespace Rx;
using namespace kms;
using namespace std;


void Compositor::init() {

    string dev_path = "/dev/dri/card0";
    Card card(dev_path);
    cout << card.get_connected_pipelines().front().connector->fullname();
    auto conns = card.get_connectors();

    for (Connector* conn : conns) {
        if (!conn->connected()) {
            printf("Connector %u not connected\n", conn->idx());
            continue;
        }

        printf("Connector %u: %sblank\n", conn->idx(), "un");
//            int r = conn->set_prop_value("DPMS", blank ? 3 : 0);
//            if (r)
//                EXIT("Failed to set DPMS: %d", r);



    }


}