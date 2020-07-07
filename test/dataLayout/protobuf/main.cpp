/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-07 22:07:39
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-07 22:17:48
 */

#include <iostream>
#include <string>

#include "addressbook.pb.h"

void BookAdd(tutorial::Person *person) {
  person->set_email("fun@qq.com");
  person->set_name("fun_name");
  person->set_id(1111);
  tutorial::Person::PhoneNumber *phone_num = person->add_phones();
  phone_num->set_number("1999");
}

int main() {
  tutorial::Person test;
  test.set_id(2111);
  test.set_name("main_name");
  test.set_email("main@qq.com");

  tutorial::Person::PhoneNumber phone_num;
  phone_num.set_number("2119");
  phone_num.set_type(tutorial::Person::WORK);

  //  tutorial::Person_PhoneNumber phone;    //等价上面的phone_num
  //  phone.set_number("2119");
  //  phone.set_type(tutorial::Person_PhoneType::Person_PhoneType_HOME);
  //  std::cout<<phone.number()<<std::endl;
  //  std::cout<<phone.type()<<std::endl;

  tutorial::AddressBook book;
  BookAdd(book.mutable_people());
  //  book.mutable_people()->set_name("main2test");    //与BookAdd函数调用等价

  std::cout << "main id :" << test.id() << std::endl;
  std::cout << "main name :" << test.name() << std::endl;
  std::cout << "main email :" << test.email() << std::endl;
  std::cout << "main phone :" << phone_num.number() << std::endl;
  std::cout << "main phone type :" << phone_num.type() << std::endl;

  const tutorial::Person &person = book.people();
  std::cout << "AddBook id :" << person.id() << std::endl;
  std::cout << "AddBook name :" << person.name() << std::endl;
  std::cout << "AddBook email :" << person.email() << std::endl;
  const tutorial::Person::PhoneNumber &num_phone = person.phones(0);
  std::cout << "AddBook phone :" << num_phone.number() << std::endl;
  std::cout << "AddBook phone type:" << num_phone.type() << std::endl;
  return 0;
}