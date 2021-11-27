# test_deferred_transaction

This is for testing deferred transaction in EOS

To start a deferred transaction:
`cleos --verbose push action account test '[false, 10]' -p actor`

 - parameters:
  + false(or true): deferred transaction will NOT have mistakes inside
  + 10: seconds to be deferred (delayed)


To check the deferred transaction:
`cleos --verbose push action account checkstate '[]' -p actor`


Result:
- defer 10 secs, and the sent deferred action does not have mistake.
 -> checkstate() in 10 seconds will return false - meaning deferred actions has not been executed.
 -> checkstate() after 10 seconds will return true - meaning it has been executed successfully.

- deferred 10 secs, and the sent deferred action is deteemined to have mistakes.
 -> checkstate() always returns false as deferred transaction is silently dead inside the defer process.

Conclusion:
- A delayed transaction is as well the so called 'deferred transaction'.
- Once the transaction 'destined' to happen has mistakes inside, it fails silently.
- std::make_tuple is used to pack action's parameters, and it is strictly required to have parameter's type matched.
- Warnings/error messages that guide developers to debug are pointless in deferered/delayed action (which will be a silently dead deferred transaction).
