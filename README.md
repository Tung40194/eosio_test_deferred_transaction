This repo is for testing deferred & inline transaction in EOS

## test_deferred_transaction

# To start a deferred transaction:
`cleos --verbose push action account testdefer '[false, 10]' -p actor`

 - parameters:
  + false(or true): deciding deferred transaction will (or will NOT) have mistakes inside
  + 10: seconds to be deferred (delayed)


# To check the deferred transaction:
`cleos --verbose push action account checkstate '[]' -p actor`


# Result:
- In case deferred in 10 secs, and the sent deferred action does not have mistake.
 -> checkstate() in 10 seconds will return false - meaning deferred actions has not been executed.
 -> checkstate() after 10 seconds will return true - meaning it has been executed successfully.

- In case deferred in 10 secs, and the sent deferred action is determined to have mistakes inside.
 -> checkstate() always returns false as deferred transaction is silently dead inside the defer-process.

# Conclusion:
- A delayed transaction is as well the so called 'deferred transaction'.
- Once the transaction 'destined' to happen has mistakes inside, it fails silently.
- std::make_tuple is used to pack action's parameters, and it is strictly required to have parameter's type matched.
- Warnings/error messages that guide developers to debug are pointless in deferered/delayed action (which will be a silently dead deferred transaction).

## test_inline_transaction
Similar to testing defer transaction, use `testinline(bool will-have-error-inside)` to test.

# Result:
- As expected, nothing is elusive or silent in the flow of execution.

# Conclusion:
- Inline action is very much just like a normal action call in many other programming platform.
