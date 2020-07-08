#include <iostream>

#include "addressbook.pb.h"

using namespace std;

int main() {
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  // 创建一个 person 对象并赋值
  tutorial::Person input_person;
  input_person.set_name("john");
  input_person.set_id(123);
  input_person.set_email("123@163.com");
  ::tutorial::Person_PhoneNumber* input_person_phone = input_person.add_phone();
  input_person_phone->set_type(tutorial::Person_PhoneType_WORK);
  input_person_phone->set_number("15112345678");

  // Serialize person to byte array
  size_t size = input_person.ByteSizeLong();
  char* data = new char[size];
  input_person.SerializeToArray(data, size);
  // Parse person from byte array
  tutorial::Person output_person;
  output_person.ParseFromArray(data, size);

  // output person info
  cout << "name: " << output_person.name() << endl;
  cout << "id: " << output_person.id() << endl;
  cout << "email: " << output_person.email() << endl;
  const ::google::protobuf::RepeatedPtrField< ::tutorial::Person_PhoneNumber>&
      output_person_phones = output_person.phone();
  for (::tutorial::Person_PhoneNumber output_person_phone :
       output_person_phones) {
    cout << "phone type: " << output_person_phone.type() << endl;
    cout << "phone number: " << output_person_phone.number() << endl;
  }

  // free data
  delete data;

  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();
  return 0;
}