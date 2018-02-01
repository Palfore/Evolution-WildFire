#include "UserFunction.h"

UserFunction::UserFunction() : specialKey(-1), key('\0'), action([](){}), release([](){}) {}
UserFunction::UserFunction(char key_t, Action action_t) : specialKey(-1), key(key_t), action(action_t), release([](){}) {}
UserFunction::UserFunction(int specialKey_t, Action action_t) : specialKey(specialKey_t), key('\0'), action(action_t), release([](){}) {}
UserFunction::UserFunction(int specialKey_t, Action action_t, Action release_t) : specialKey(specialKey_t), key('\0'), action(action_t), release(release_t) {
}

UserFunction::~UserFunction() {
}
