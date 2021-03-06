#include <iostream>
#include <vector>

using namespace std;

class MyDData
{
  public:
    const int foo;
    int bar;
    MyDData(const int& _initBar ) : foo(_initBar)
    {
        bar = _initBar; //some cool operation to set up bar
    }
    void update(const int& _newbar) { bar = _newbar;}
    string get(){ return "foo: " + to_string(foo) + " bar: " + to_string(bar);}

};

class A
{
 protected:
    const string s;
    MyDData d;

 public:
    A( const std::string _s, int _i ): s( _s ), d( _i ) { cout << "Constructor A: " << s <<endl; }
    virtual ~A(void) { cout << "destructor A" << endl; }
    virtual void f() { cout << "A::f()" << endl; }
    virtual void g() { cout << "A::g()" << endl; }
    virtual string get(){ return "s: " + s + "d: (" + d.get() +")";}
};


class BA : public A
{
  protected:
    virtual string k() = 0;
  public:
    BA( const std::string _s, int _i) : A( _s, _i) { cout << "Constructor B: " << endl; }
    void h() { cout << "B::h()" << endl; }
};

class CBA : public BA
{
    int foobar;
  public:
    CBA( const std::string _s, int _i ) : BA( _s, _i ), foobar(42) { cout << "Constructor C (foobar default):"; l(); }
    CBA( const std::string _s, int _i, int _fb ) : BA( _s, _i ), foobar(_fb) { cout << "Constructor C (foobar parameter):"; l(); }
  private:
    string k() {return "CBA needs to implement BA::k() because it is pure virtual";}
    void l() { cout << "CBA::l()" << endl; }
};

class DA : public A
{
    vector<string> s_vec;
    BA* b_ptr;
public:
    DA( const std::string _s, int _i ) : A ( _s,  _i ) {}
    void m() { cout << "DA::m()" << endl; }
    ~DA(){ cout << "destructor DA" <<endl; }
};

void show(const string who, const string info)
{
    cout << "MAIN::"+ who +" info: "+ info << endl;
}
int main( int argc, const char** argv )
{
    MyDData d(33);
    show("d", d.get());
    d.update(99);
    show("d", d.get());

    CBA cba("cbaName", 100);
    show("cba", cba.get());
    DA da("cbaName", 200);
    show("da", da.get());
}


