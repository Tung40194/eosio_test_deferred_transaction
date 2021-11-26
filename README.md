# test_deferred_transaction

This is made to test deferred transaction in EOS

To start a deferred transaction:
`cleos --verbose push action account test '[false, 10]' -p actor`
 
 + false(or true): decide that the deferred action will NOT have mistakes inside
 + 10: seconds to be deferred (delayed)


To check the deferred transaction:
`cleos --verbose push action account checkstate '[]' -p actor`


Result:
- defer 10 secs, and the sent deferred action does not have mistake.
 -> checkstate() in 10 seconds will return false - meaning deferred actions has not been executed.
 -> checkstate() after 10 seconds will return true - meaning it has been executed successfully.

- deferred 10 secs, and the sent deferred action is deteemined to have mistakes.
 -> checkstate() always returns false as deferred transaction is silently dead inside the defer process.
