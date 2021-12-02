#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/singleton.hpp>
#include <eosio/permission.hpp>

using namespace eosio;
using namespace std;

CONTRACT testdeferred : public contract {
public:

    testdeferred(eosio::name receiver, eosio::name code, datastream<const char *> ds) : 
        contract(receiver, code, ds),
        state_table(_self, _self.value)	{
            // constructor
            eosio::print(">>> running contructor\n");
        }

    ACTION initstate();
    // test if the deferred TX has mistake inside
    ACTION testdefer(bool will_error_be_in_deferred_tx, uint64_t delay_in_sec);
    // test if the inline TX has mistake inside
    ACTION testinline(bool will_error_be_in_deferred_tx);
    //the action to be deferred/inlined
    ACTION subtest(bool err, string mark);
    ACTION checkstate();

    TABLE state {
    	bool run = false; //has defererd action run?
	EOSLIB_SERIALIZE( state, (run));
    };
    typedef eosio::singleton<"state.inf"_n, state> state_info_table;
    typedef eosio::multi_index<"state.inf"_n, state> fstate_info_table;

    state_info_table state_table;

};

ACTION testdeferred::initstate() {
	state_table.set(state{true}, _self);
}

// test if defered TX has mistake inside
ACTION testdeferred::testdefer(bool will_error_be_in_deferred_tx, uint64_t delay_in_sec) {
	eosio::transaction autoExec;

	//std::make_tuple has a very strict way in taking in parameters, make sure parameters' types match
	//for more: https://eosio.stackexchange.com/questions/4577/on-method-call-assertion-failure-with-message-read
	bool err = will_error_be_in_deferred_tx;
	std::string mark = ">>>running subtest\n";
	autoExec.actions.emplace_back(eosio::permission_level{_self, "active"_n}, _self, "subtest"_n, std::make_tuple(err, mark));
	autoExec.delay_sec = delay_in_sec;
	eosio::print(">>> running test function\n");
    	autoExec.send(_self.value, _self, true);
}

ACTION testdeferred::testinline(bool will_error_be_in_deferred_tx) {
        bool err = will_error_be_in_deferred_tx;
        std::string mark = ">>>running subtest\n";

        action(permission_level{_self, "active"_n},
                        _self,
                        "subtest"_n,
                        std::make_tuple(err, mark)).send();
}

ACTION testdeferred::subtest(bool err, string mark) {
	eosio::print(mark);
	if (err) {
		// expect program to stop here, silently in defered tx(subtest)
		check(0==1, "cmp 0 and 1");
	}
	else {
		// expect no error here, program should go to 'mark as run state'
		check(1==1, "cmp 1 and 1");
	}

	// mark as run in state table
	state_table.set(state{true}, _self);
}

ACTION testdeferred::checkstate() {
	//check
	check(state_table.exists(), "ERROR: state table has not been initialized.");
        auto state_info = state_table.get();
	eosio::print(">>> check run state: ", state_info.run);
	//reset
	state_table.set(state{false}, _self);
}
