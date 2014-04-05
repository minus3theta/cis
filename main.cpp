#include <iostream>
using namespace std;

class cons;

union data {
    int n;
    char *s;
    cons *c;
};

const int DATA_INVALID = 0;
const int DATA_INT = 1;
const int DATA_SYMBOL = 2;
const int DATA_STRING = 3;
const int DATA_CONS = 4;
const int DATA_EMPTY_LIST = 100;

class obj {
    int type;
    data d;
public:
    obj();
    obj(int t, char *str);
    obj(obj *car, obj *cdr);
    ~obj();
    void show();
};

obj *EMPTY_LIST = new obj();

class cons {
    obj *car;
    obj *cdr;
public:
    cons(obj *car, obj *cdr);
    ~cons();
    void show();
};

obj::obj() {
    type = DATA_EMPTY_LIST;
}

obj::obj(int t, char *str) {
    if(t == DATA_STRING || t == DATA_SYMBOL) {
        type = t;
        d.s = str;
    } else {
        type = DATA_INVALID;
    }
}

obj::obj(obj *car, obj *cdr) {
    type = DATA_CONS;
    d.c = new cons(car, cdr);
}

obj::~obj() {
    if(type == DATA_CONS) {
        delete d.c;
    } else if(type == DATA_SYMBOL || type == DATA_STRING) {
        delete d.s;
    }
}

void obj::show() {
    if(this == EMPTY_LIST) {
        cout << " ";
        return;
    }
    switch(type) {
    case DATA_STRING:
        cout << "*\"" << d.s << '"';
        break;
    case DATA_SYMBOL:
        cout << d.s;
        break;
    case DATA_CONS:
        d.c->show();
        break;
    }
}

cons::cons(obj *car, obj *cdr) {
    this->car = car;
    this->cdr = cdr;
}

cons::~cons() {
    delete car;
    delete cdr;
}

void cons::show() {
    cout << "(";
    car->show();
    cout << ".";
    cdr->show();
    cout << ")";
}

obj *analize(const char *str, int &start);

obj *getPrimitive(const char *str, int &start) {
    int type;
    const char *s = str + start;
    int length = 0;
    const char *p;
    if(*s == '"') {
        type = DATA_STRING;
        s++;
        start++;
        p = s;
        for(; *s && *s!='"'; s++) {
            length++;
        }
        start += length;
        while(*s && (*s == ' ' || *s == '"')) {
            s++;
            start++;
        }
    } else {
        type = DATA_SYMBOL;
        p = s;
        for(; *s && *s!=' ' && *s!=')'; s++) {
            length++;
        }
        start += length;
        while(*s && *s == ' ') {
            s++;
            start++;
        }
    }
    char *q = new char[length+1];
    for(int i=0; i<length; i++) {
        q[i] = p[i];
    }
    q[length] = '\0';
    return new obj(type, q);
}

obj *getList(const char *str, int &start) {
    if(!str[start] || str[start] == ')') {
        start++;
        return EMPTY_LIST;
    }
    return new obj(analize(str, start), getList(str, start));
}

obj *analize(const char *str, int &start) {
    while(!str[start] || str[start] == ' ') start++;
    if(str[start] == '(') {
        return getList(str, ++start);
    } else {
        return getPrimitive(str, start);
    }
}
    

int main() {
    char str[100] = "((\"(foo bar)\" hoge) baz)";
    int start = 0;
    obj *exp = analize(str, start);
    exp->show();
    cout << endl;
    return 0;
}
