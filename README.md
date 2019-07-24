# cls2json
- Generate JSON from the class file of Java Virtual Machine.
- Supported version: Java SE 11 Edition

# Build
```Shell
$ mkdir build
$ cd build
$ cmake .. 
$ make
```

# Usage
```Shell
$ cls2json classfile...
```

# Example
Create `Hello.class` from Hello.java, then use it as an argument.
```Hello.java
public class Hello {
    public static void main(String[] args) {
        System.out.println("Hello, World.");
    }
}
```

```Shell
$ javac Hello.java
$ ./cls2json Hello.class 
{"magic":"0xcafebabe","minor_version":0,"major_version":55,"constant_pool_count":29,"constant_pool":["null",{"tag":10,"class_index":6,"name_and_type_index":15},{"tag":9,"class_index":16,"name_and_type_index":17},{"tag":8,"string_index":18},{"tag":10,"class_index":19,"name_and_type_index":20},{"tag":7,"name_index":21},{"tag":7,"name_index":22},{"tag":1,"length":6,"bytes":"<init>"},{"tag":1,"length":3,"bytes":"()V"},{"tag":1,"length":4,"bytes":"Code"},{"tag":1,"length":15,"bytes":"LineNumberTable"},{"tag":1,"length":4,"bytes":"main"},{"tag":1,"length":22,"bytes":"([Ljava/lang/String;)V"},{"tag":1,"length":10,"bytes":"SourceFile"},{"tag":1,"length":10,"bytes":"Hello.java"},{"tag":12,"name_index":7,"descriptor_index":8},{"tag":7,"name_index":23},{"tag":12,"name_index":24,"descriptor_index":25},{"tag":1,"length":13,"bytes":"Hello, World."},{"tag":7,"name_index":26},{"tag":12,"name_index":27,"descriptor_index":28},{"tag":1,"length":5,"bytes":"Hello"},{"tag":1,"length":16,"bytes":"java/lang/Object"},{"tag":1,"length":16,"bytes":"java/lang/System"},{"tag":1,"length":3,"bytes":"out"},{"tag":1,"length":21,"bytes":"Ljava/io/PrintStream;"},{"tag":1,"length":19,"bytes":"java/io/PrintStream"},{"tag":1,"length":7,"bytes":"println"},{"tag":1,"length":21,"bytes":"(Ljava/lang/String;)V"}],"access_Flags":"0x33","this_class":5,"super_class":6,"interfaces_count":0,"interfaces":[],"fields_count":0,"fields":[],"methods_count":2,"methods":[{"access_flags":1,"name_index":7,"descriptor_index":8,"attributes_count":1,"attributes":[{"attribute_name_index":9,"attribute_length":29,"max_stack":1,"max_locals":1,"code_length":5,"code":[42,183,0,1,177],"exception_table_length":0,"exception_table":[],"attributes_count":1,"attributes":[{"attribute_name_index":10,"attribute_length":6,"line_number_table_length":1,"line_number_table":[{"start_pc":0,"line_number":1}]}]}]}, {"access_flags":9,"name_index":11,"descriptor_index":12,"attributes_count":1,"attributes":[{"attribute_name_index":9,"attribute_length":37,"max_stack":2,"max_locals":1,"code_length":9,"code":[178,0,2,18,3,182,0,4,177],"exception_table_length":0,"exception_table":[],"attributes_count":1,"attributes":[{"attribute_name_index":10,"attribute_length":10,"line_number_table_length":2,"line_number_table":[{"start_pc":0,"line_number":3},{"start_pc":8,"line_number":4}]}]}]}],"attributes_count":1,"attributes":[{"attribute_name_index":13,"attribute_length":2,"source_file_index":14}]}
```

Convert string to a human-readable format by using `jg`
```Shell
$ ./cls2json Hello.class | jq
{
  "magic": "0xcafebabe",
  "minor_version": 0,
  "major_version": 55,
  "constant_pool_count": 29,
  "constant_pool": [
    "null",
    {
      "tag": 10,
      "class_index": 6,
      "name_and_type_index": 15
    },
    {
      "tag": 9,
      "class_index": 16,
      "name_and_type_index": 17
    },
    {
      "tag": 8,
      "string_index": 18
    },
    {
      "tag": 10,
      "class_index": 19,
      "name_and_type_index": 20
    },
    {
      "tag": 7,
      "name_index": 21
    },
    {
      "tag": 7,
      "name_index": 22
    },
    {
      "tag": 1,
      "length": 6,
      "bytes": "<init>"
    },
    {
      "tag": 1,
      "length": 3,
      "bytes": "()V"
    },
    {
      "tag": 1,
      "length": 4,
      "bytes": "Code"
    },
    {
      "tag": 1,
      "length": 15,
      "bytes": "LineNumberTable"
    },
    {
      "tag": 1,
      "length": 4,
      "bytes": "main"
    },
    {
      "tag": 1,
      "length": 22,
      "bytes": "([Ljava/lang/String;)V"
    },
    {
      "tag": 1,
      "length": 10,
      "bytes": "SourceFile"
    },
    {
      "tag": 1,
      "length": 10,
      "bytes": "Hello.java"
    },
    {
      "tag": 12,
      "name_index": 7,
      "descriptor_index": 8
    },
    {
      "tag": 7,
      "name_index": 23
    },
    {
      "tag": 12,
      "name_index": 24,
      "descriptor_index": 25
    },
    {
      "tag": 1,
      "length": 13,
      "bytes": "Hello, World."
    },
    {
      "tag": 7,
      "name_index": 26
    },
    {
      "tag": 12,
      "name_index": 27,
      "descriptor_index": 28
    },
    {
      "tag": 1,
      "length": 5,
      "bytes": "Hello"
    },
    {
      "tag": 1,
      "length": 16,
      "bytes": "java/lang/Object"
    },
    {
      "tag": 1,
      "length": 16,
      "bytes": "java/lang/System"
    },
    {
      "tag": 1,
      "length": 3,
      "bytes": "out"
    },
    {
      "tag": 1,
      "length": 21,
      "bytes": "Ljava/io/PrintStream;"
    },
    {
      "tag": 1,
      "length": 19,
      "bytes": "java/io/PrintStream"
    },
    {
      "tag": 1,
      "length": 7,
      "bytes": "println"
    },
    {
      "tag": 1,
      "length": 21,
      "bytes": "(Ljava/lang/String;)V"
    }
  ],
  "access_Flags": "0x33",
  "this_class": 5,
  "super_class": 6,
  "interfaces_count": 0,
  "interfaces": [],
  "fields_count": 0,
  "fields": [],
  "methods_count": 2,
  "methods": [
    {
      "access_flags": 1,
      "name_index": 7,
      "descriptor_index": 8,
      "attributes_count": 1,
      "attributes": [
        {
          "attribute_name_index": 9,
          "attribute_length": 29,
          "max_stack": 1,
          "max_locals": 1,
          "code_length": 5,
          "code": [
            42,
            183,
            0,
            1,
            177
          ],
          "exception_table_length": 0,
          "exception_table": [],
          "attributes_count": 1,
          "attributes": [
            {
              "attribute_name_index": 10,
              "attribute_length": 6,
              "line_number_table_length": 1,
              "line_number_table": [
                {
                  "start_pc": 0,
                  "line_number": 1
                }
              ]
            }
          ]
        }
      ]
    },
    {
      "access_flags": 9,
      "name_index": 11,
      "descriptor_index": 12,
      "attributes_count": 1,
      "attributes": [
        {
          "attribute_name_index": 9,
          "attribute_length": 37,
          "max_stack": 2,
          "max_locals": 1,
          "code_length": 9,
          "code": [
            178,
            0,
            2,
            18,
            3,
            182,
            0,
            4,
            177
          ],
          "exception_table_length": 0,
          "exception_table": [],
          "attributes_count": 1,
          "attributes": [
            {
              "attribute_name_index": 10,
              "attribute_length": 10,
              "line_number_table_length": 2,
              "line_number_table": [
                {
                  "start_pc": 0,
                  "line_number": 3
                },
                {
                  "start_pc": 8,
                  "line_number": 4
                }
              ]
            }
          ]
        }
      ]
    }
  ],
  "attributes_count": 1,
  "attributes": [
    {
      "attribute_name_index": 13,
      "attribute_length": 2,
      "source_file_index": 14
    }
  ]
}
```
