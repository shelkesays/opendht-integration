#include <opendht.h>
#include <vector>

int main()
{
    dht::DhtRunner node1;
    dht::DhtRunner node2;

    node1.run(4222, dht::crypto::generateIdentity(), true);
    node2.run(4232, dht::crypto::generateIdentity(), true);

    node2.bootstrap(node1.getBound());

    std::cout << "Node 1 Port: " << node1.getBoundPort() << std::endl;
    std::cout << "Node 2 Port: " << node2.getBoundPort() << std::endl;

    auto node_id = node1.getNodeId();

    std::cout << "Node 1 id: " << node_id << std::endl;

    auto key = dht::InfoHash::get("123");
    dht::Value val {"hey"};

    auto val_data = val.data;
    std::cout << val << std::endl;
    std::promise<bool> p;
    node2.put(key, std::move(val), [&](bool ok){
        p.set_value(ok);
    });

    std::cout << "Node 2 Get: " <<  node2.get(key).get() << std::endl;

    std::cout << "Node 2 Promise: " <<  p.get_future().get() << std::endl;

    auto vals = node1.get(key).get();

    std::cout << "Node 1 get data: " <<  vals.data() << std::endl;
    
    std::atomic_uint valueCount(0);
    auto a = dht::InfoHash::get("234");
    auto ftokena = node1.listen(a, [&](const std::shared_ptr<dht::Value>&) {
        valueCount++;
        return true;
    });

    // Join the network through any running node,
    // here using a known bootstrap node.
    // node.bootstrap("bootstrap.ring.cx", "4222");

    // put some data on the dht
    // std::vector<uint8_t> some_data(5, 10);
    // node.put("unique_key", some_data);

    // put some data on the dht, signed with our generated private key
    // node.putSigned("unique_key_42", some_data);

    // get data from the dht
    // node.get("other_unique_key", [](const std::vector<std::shared_ptr<dht::Value>>& values) {
        // Callback called when values are found
        // for (const auto& value : values)
            // std::cout << "Found value: " << *value << std::endl;
        // return true; // return false to stop the search
    // });

    // wait for dht threads to end
    node1.join();
    node2.join();
    return 0;
}