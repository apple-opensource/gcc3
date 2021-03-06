// 2001-09-09 Benjamin Kosnik  <bkoz@redhat.com>

// Copyright (C) 2001, 2002 Free Software Foundation
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
// USA.

// 22.2.6.2.1 money_put members

#include <locale>
#include <sstream>
#include <testsuite_hooks.h>

// XXX This test is not working for non-glibc locale models.
// { dg-do run { xfail *-*-* } }

#ifdef _GLIBCPP_USE_WCHAR_T
// test string version
void test01()
{
  using namespace std;
  typedef money_base::part part;
  typedef money_base::pattern pattern;
  typedef ostreambuf_iterator<wchar_t> iterator_type;

  bool test = true;

  // basic construction
  locale loc_c = locale::classic();
  locale loc_hk("en_HK");
  locale loc_fr("fr_FR@euro");
  locale loc_de("de_DE");
  VERIFY( loc_c != loc_de );
  VERIFY( loc_hk != loc_fr );
  VERIFY( loc_hk != loc_de );
  VERIFY( loc_de != loc_fr );

  // cache the moneypunct facets
  typedef moneypunct<wchar_t, true> __money_true;
  typedef moneypunct<wchar_t, false> __money_false;
  const __money_true& monpunct_c_t = use_facet<__money_true>(loc_c); 
  const __money_true& monpunct_de_t = use_facet<__money_true>(loc_de); 
  const __money_false& monpunct_c_f = use_facet<__money_false>(loc_c); 
  const __money_false& monpunct_de_f = use_facet<__money_false>(loc_de); 
  const __money_true& monpunct_hk_t = use_facet<__money_true>(loc_hk); 
  const __money_false& monpunct_hk_f = use_facet<__money_false>(loc_hk); 

  // sanity check the data is correct.
  const wstring empty;

  // total EPA budget FY 2002
  const wstring digits1(L"720000000000");

  // est. cost, national missile "defense", expressed as a loss in USD 2001
  const wstring digits2(L"-10000000000000");  

  // not valid input
  const wstring digits3(L"-A"); 

  // input less than frac_digits
  const wstring digits4(L"-1");
  
  wostringstream oss;
  oss.imbue(loc_de);
  // cache the money_put facet
  const money_put<wchar_t>& mon_put = use_facet<money_put<wchar_t> >(oss.getloc()); 


  iterator_type os_it01 = mon_put.put(oss.rdbuf(), true, oss, ' ', digits1);
  wstring result1 = oss.str();
  VERIFY( result1 == L"7.200.000.000,00 ");

  oss.str(empty);
  iterator_type os_it02 = mon_put.put(oss.rdbuf(), false, oss, ' ', digits1);
  wstring result2 = oss.str();
  VERIFY( result2 == L"7.200.000.000,00 ");

  // intl and non-intl versions should be the same.
  VERIFY( result1 == result2 );

  // now try with showbase, to get currency symbol in format
  oss.setf(ios_base::showbase);

  oss.str(empty);
  iterator_type os_it03 = mon_put.put(oss.rdbuf(), true, oss, ' ', digits1);
  wstring result3 = oss.str();
  VERIFY( result3 == L"7.200.000.000,00 DEM ");

  oss.str(empty);
  iterator_type os_it04 = mon_put.put(oss.rdbuf(), false, oss, ' ', digits1);
  wstring result4 = oss.str();
  VERIFY( result4 == L"7.200.000.000,00 DM");

  // intl and non-intl versions should be different.
  VERIFY( result3 != result4 );
  VERIFY( result3 != result1 );
  VERIFY( result4 != result2 );

  // test sign of more than one digit, say hong kong.
  oss.imbue(loc_hk);
  oss.str(empty);
  iterator_type os_it05 = mon_put.put(oss.rdbuf(), false, oss, ' ', digits1);
  wstring result5 = oss.str();
  VERIFY( result5 == L"HK$7,200,000,000.00");

  oss.str(empty);
  iterator_type os_it06 = mon_put.put(oss.rdbuf(), true, oss, ' ', digits2);
  wstring result6 = oss.str();
  VERIFY( result6 == L"(HKD 100,000,000,000.00)");

  // test one-digit formats without zero padding
  oss.imbue(loc_c);
  oss.str(empty);
  iterator_type os_it07 = mon_put.put(oss.rdbuf(), true, oss, ' ', digits4);
  wstring result7 = oss.str();
  VERIFY( result7 == L"1");

  // test one-digit formats with zero padding, zero frac widths
  oss.imbue(loc_hk);
  oss.str(empty);
  iterator_type os_it08 = mon_put.put(oss.rdbuf(), true, oss, ' ', digits4);
  wstring result8 = oss.str();
  VERIFY( result8 == L"(HKD .01)");

  oss.unsetf(ios_base::showbase);

  // test bunk input
  oss.str(empty);
  iterator_type os_it09 = mon_put.put(oss.rdbuf(), true, oss, ' ', digits3);
  wstring result9 = oss.str();
  VERIFY( result9 == L"");

  // test io.width() > length
  // test various fill strategies
  oss.imbue(loc_de);

  oss.str(empty);
  oss.width(20);
  iterator_type os_it10 = mon_put.put(oss.rdbuf(), true, oss, '*', digits4);
  wstring result10 = oss.str();
  VERIFY( result10 == L"***************-,01*");

  oss.str(empty);
  oss.width(20);
  oss.setf(ios_base::internal);
  iterator_type os_it11 = mon_put.put(oss.rdbuf(), true, oss, '*', digits4);
  wstring result11 = oss.str();
  VERIFY( result11 == L"-,01****************");
}

// test double/wstring versions
void test02()
{
  using namespace std;
  typedef money_base::part part;
  typedef money_base::pattern pattern;
  typedef ostreambuf_iterator<wchar_t> iterator_type;

  bool test = true;

  // basic construction
  locale loc_c = locale::classic();
  locale loc_hk("en_HK");
  locale loc_fr("fr_FR@euro");
  locale loc_de("de_DE");
  VERIFY( loc_c != loc_de );
  VERIFY( loc_hk != loc_fr );
  VERIFY( loc_hk != loc_de );
  VERIFY( loc_de != loc_fr );

  // cache the moneypunct facets
  typedef moneypunct<wchar_t, true> __money_true;
  typedef moneypunct<wchar_t, false> __money_false;
  const __money_true& monpunct_c_t = use_facet<__money_true>(loc_c); 
  const __money_true& monpunct_de_t = use_facet<__money_true>(loc_de); 
  const __money_false& monpunct_c_f = use_facet<__money_false>(loc_c); 
  const __money_false& monpunct_de_f = use_facet<__money_false>(loc_de); 
  const __money_true& monpunct_hk_t = use_facet<__money_true>(loc_hk); 
  const __money_false& monpunct_hk_f = use_facet<__money_false>(loc_hk); 

  // sanity check the data is correct.
  const wstring empty;

  // total EPA budget FY 2002
  const long double  digits1 = 720000000000;

  // est. cost, national missile "defense", expressed as a loss in USD 2001
  const long double digits2 = -10000000000000;  

  // input less than frac_digits
  const long double digits4 = -1;
  

  wostringstream oss;
  oss.imbue(loc_de);
  // cache the money_put facet
  const money_put<wchar_t>& mon_put = use_facet<money_put<wchar_t> >(oss.getloc()); 


  iterator_type os_it01 = mon_put.put(oss.rdbuf(), true, oss, ' ', digits1);
  wstring result1 = oss.str();
  VERIFY( result1 == L"7.200.000.000,00 ");

  oss.str(empty);
  iterator_type os_it02 = mon_put.put(oss.rdbuf(), false, oss, ' ', digits1);
  wstring result2 = oss.str();
  VERIFY( result2 == L"7.200.000.000,00 ");

  // intl and non-intl versions should be the same.
  VERIFY( result1 == result2 );

  // now try with showbase, to get currency symbol in format
  oss.setf(ios_base::showbase);

  oss.str(empty);
  iterator_type os_it03 = mon_put.put(oss.rdbuf(), true, oss, ' ', digits1);
  wstring result3 = oss.str();
  VERIFY( result3 == L"7.200.000.000,00 DEM ");

  oss.str(empty);
  iterator_type os_it04 = mon_put.put(oss.rdbuf(), false, oss, ' ', digits1);
  wstring result4 = oss.str();
  VERIFY( result4 == L"7.200.000.000,00 DM");

  // intl and non-intl versions should be different.
  VERIFY( result3 != result4 );
  VERIFY( result3 != result1 );
  VERIFY( result4 != result2 );
}

void test03()
{
  using namespace std;
  bool test = true;

  // Check money_put works with other iterators besides streambuf
  // output iterators. (As long as output_iterator requirements are met.)
  typedef wstring::iterator iter_type;
  typedef money_put<wchar_t, iter_type> mon_put_type;
  const ios_base::iostate goodbit = ios_base::goodbit;
  const ios_base::iostate eofbit = ios_base::eofbit;
  ios_base::iostate err = goodbit;
  const locale loc_c = locale::classic();
  // woman, art, thief (stole the blues)
  const wstring str(L"1943 Janis Joplin");
  const long double ld = 1943;
  const wstring x(str.size(), 'x'); // have to have allocated string!
  wstring res;

  wostringstream oss; 
  oss.imbue(locale(loc_c, new mon_put_type));

  // Iterator advanced, state, output.
  const mon_put_type& mp = use_facet<mon_put_type>(oss.getloc());

  // 01 string
  res = x;
  iter_type ret1 = mp.put(res.begin(), false, oss, ' ', str);
  wstring sanity1(res.begin(), ret1);
  VERIFY( err == goodbit );
  VERIFY( res == L"1943xxxxxxxxxxxxx" );
  VERIFY( sanity1 == L"1943" );

  // 02 long double
  res = x;
  iter_type ret2 = mp.put(res.begin(), false, oss, ' ', ld);
  wstring sanity2(res.begin(), ret2);
  VERIFY( err == goodbit );
  VERIFY( res == L"1943xxxxxxxxxxxxx" );
  VERIFY( sanity2 == L"1943" );
}

// libstdc++/5280
void test04()
{
#ifdef _GLIBCPP_HAVE_SETENV 
  // Set the global locale to non-"C".
  std::locale loc_de("de_DE");
  std::locale::global(loc_de);

  // Set LANG environment variable to de_DE.
  const char* oldLANG = getenv("LANG");
  if (!setenv("LANG", "de_DE", 1))
    {
      test01();
      test02();
      test03();
      setenv("LANG", oldLANG ? oldLANG : "", 1);
    }
#endif
}

struct My_money_io : public std::moneypunct<wchar_t,false>
{
  char_type do_decimal_point() const { return L'.'; }
  char_type do_thousands_sep() const { return L','; }
  std::string do_grouping() const { return "\003"; }
  
  std::wstring do_negative_sign() const { return L"()"; }
  
  int do_frac_digits() const { return 2; }

  pattern do_neg_format() const
  {
    pattern pat = { { symbol, space, sign, value } };
    return pat;
  }
};

// libstdc++/5708
void test05()
{
  using namespace std;
  typedef ostreambuf_iterator<wchar_t> OutIt;

  locale loc(locale::classic(), new My_money_io);

  bool intl = false;

  wstring val(L"-123456");
  const money_put<wchar_t,OutIt>& mp  =
    use_facet<money_put<wchar_t, OutIt> >(loc);

  wostringstream fmt;
  fmt.imbue(loc);
  OutIt out(fmt);
  mp.put(out,intl,fmt,L'*',val);
  VERIFY( fmt.str() == L"*(1,234.56)" );
}

struct My_money_io_2 : public std::moneypunct<wchar_t,false>
{
  char_type do_thousands_sep() const { return L','; }
  std::string do_grouping() const { return "\001"; }
};

// Make sure we can output a very big amount of money (with grouping too).
void test06()
{
  using namespace std;
  typedef ostreambuf_iterator<wchar_t> OutIt;

  locale loc(locale::classic(), new My_money_io_2);

  bool intl = false;

  long double val = 1e50L;
  const money_put<wchar_t,OutIt>& mp  =
    use_facet<money_put<wchar_t, OutIt> >(loc);

  wostringstream fmt;
  fmt.imbue(loc);
  OutIt out(fmt);
  mp.put(out,intl,fmt,'*',val);
  VERIFY( fmt );
}
#endif

int main()
{
#ifdef _GLIBCPP_USE_WCHAR_T
  test01();
  test02();
  test03();
  test04();
  test05();
  test06();
#endif
  return 0;
}
