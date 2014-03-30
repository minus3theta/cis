#include <iostream>
#include <stack>
using namespace std;

class cons;

union data {
    int n;
    char *s;
    cons *c;
};

const int DATA_INT = 1;
const int DATA_SYMBOL = 2;
const int DATA_STRING = 3;
const int DATA_CONS = 4;

class obj {
public:
    int type;
    data d;
    void show();
};

class cons {
public:
    obj *car;
    obj *cdr;
    void show();
};

void obj::show() {
    cout << "type: " << type << endl;
    cout << "value: ";
    if(type == DATA_SYMBOL || type == DATA_STRING) {
        cout << d.s << endl;
    } else if(type == DATA_CONS) {
        if(d.c) {
            d.c->show();
        } else {
            cout << "EMPTY_LIST" << endl;
        }
    }
}

void cons::show() {
    cout << "car: ";
    car->show();
    cout << "cdr: ";
    cdr->show();
    cout << endl;
}

static cons *EMPTY_LIST = NULL;

obj *analize(const char *str, int &start);

obj *getPrimitive(const char *str, int &start) {
    obj *ret = new obj();
    const char *s = str + start;
    if(*s == '"') {
        ret->type = DATA_STRING;
        s++;
        start++;
    } else {
        ret->type = DATA_SYMBOL;
    }
    int length = 0;
    const char *p = s;
    for(; *s && *s!=' ' && *s!=')'; s++) {
        length++;
    }
    start += length + 1;
    if(ret->type == DATA_STRING) {
        length--;
    }
    ret->d.s = new char[length+1];
    for(int i=0; i<length; i++) {
        ret->d.s[i] = p[i];
    }
    ret->d.s[length] = '\0';
    cout << ret->d.s << endl;
    return ret;
}

obj *getList(const char *str, int &start) {
    obj *ret = new obj();
    ret->type = DATA_CONS;
    if(!str[start] || str[start] == ')') {
        ret->d.c = EMPTY_LIST;
        return ret;
    }
    ret->d.c = new cons();
    ret->d.c->car = analize(str, start);
    ret->d.c->cdr = getList(str, start);
    return ret;
}

obj *analize(const char *str, int &start) {
    while(!str[start] && str[start] == ' ') start++;
    if(str[start] == '(') {
        return getList(str, ++start);
    } else {
        return getPrimitive(str, start);
    }
}
    

int main() {
    char str[100] = "(foo (bar) baz)";
    int start = 0;
    obj *exp = analize(str, start);
    exp->show();
    return 0;
}
