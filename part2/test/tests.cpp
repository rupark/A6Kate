//    Status* s = new Status(0, 1, new String("hello"));
//    cout << s->serialize()->cstr_ << endl;
//    Status* f = new Status(s->serialize()->cstr_);
//    cout << f->serialize()->cstr_ << endl;
//
//    Register* r = new Register(0, 1, 8080, new String("127.0.0.1"));
//    cout << r->serialize()->cstr_ << endl;
//    Register* e = new Register(r->serialize()->cstr_);
//    cout << e->serialize()->cstr_ << endl;
//
//    Ack* a = new Ack(0, 1);
//    cout << a->serialize()->cstr_ << endl;
//    Ack* g = new Ack(a->serialize()->cstr_);
//    cout << g->serialize()->cstr_ << endl;
//
//    size_t* ports = new size_t[3];
//    ports[0] = 1;
//    ports[1] = 1;
//    ports[2] = 1;
//    String** add = new String*[3];
//    add[0] = new String("127.0.0.1");
//    add[1] = new String("127.0.0.1");
//    add[2] = new String("127.0.0.1");
//    Directory* d = new Directory(0, 1, 3, ports, add);
//    cout << d->serialize()->cstr_ << endl;
//    Directory* c = new Directory(d->serialize()->cstr_);
//    cout << c->serialize()->cstr_ << endl;
