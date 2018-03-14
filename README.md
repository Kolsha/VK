# VK
VK API Lib - небольшая библиотека для работы с vk.com API, написанная на чистом C++. Из зависимостей требуется только Curl.
Библиотека предоставляет базовый класс `VK::Client`, с методами авторизации и выполнения запросов к API.

Есть поддержка:

* Обработки капчи

* Двух-факторной авторизации

На основе его Вы можете создавать свои классы для работы с разделами, для примера приведен класс `VK::Messages`.

Базовый пример использования:
```cpp
#include "src/api.h"
...
VK::Client api;
if(api.auth(login, pass, access_token)){
  cout << "Auth ok" << endl;
  cout << "Access token: " << api.access_token() << endl << endl;  
  cout << api.call("wall.post", "owner_id=12345&message=Test");
}
else{
  cout << "Auth fail" << endl;
}
```
