// Test that attributes weak and alias coexist.
// excess errors test - XFAIL alpha*-dec-osf* *-*-hms i?86-pc-cygwin *-*-coff

extern "C" {
  void f () __attribute__((weak, alias ("_f")));
  void _f () { }
}

int main ()
{
  f ();
}
