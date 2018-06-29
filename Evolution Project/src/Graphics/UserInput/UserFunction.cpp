#include "UserFunction.h"


void UIElement::affectState(unsigned char keyPress, CALL_TYPE type) {if (keyPress||type==CALL_TYPE::NONE) return;}
void UIElement::affectState(int specialKeyPress, CALL_TYPE type) {if (specialKeyPress||type==CALL_TYPE::NONE) return;}
void UIElement::affectState(int mx, int my, CALL_TYPE type) {if (mx==my||type==CALL_TYPE::NONE) return;}

UserFunction::UserFunction(UIElement* ele, Action action_t) : element(ele), action(action_t), release([](){}) {}
UserFunction::UserFunction(UIElement* ele, Action action_t, Action release_t) : element(ele), action(action_t), release(release_t) {}
UserFunction::~UserFunction() {/* Don't delete element because element doesnt have copy operator. Must handle freeing outside class. (laziness) */}
UserFunction& UserFunction::operator=(const UserFunction& other) {
    element = other.element;
    return *this;
}
