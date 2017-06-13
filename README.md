# VK
VK API Lib - небольшая библиотека для работы API, написанная на чистом C++. Из зависимостей требуется только Curl.
Библиотека предоставляет базовый класс <b>VK::Client</b>, с методами авторизации и выполнения запросов к API.

На основе его Вы можете создавать свои классы для работы с разделами, для примера приведен класс <b>VK::Messages</b>

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
