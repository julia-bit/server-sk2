# <div style="text-align: center">SK-2 Projekt Zaliczeniowy </br> Komunikator internetowy</div>

### <div style="text-align: right">Julia Mularczyk 148062 </br> Michał Pawlicki 148239</div>

## Zasada działania

Klient łączy się z serwerem za pomocą websocketa, przez który wysyłane są na serwer wiadomości oraz dane nowych urzytkowników, oraz drugiego socketa, który służy do pobierania wiadomości oraz informacji o użytkownikach za pomocą interfejsu REST API. Do przechowywania danych został użyty system zarządzenia bazami danych sqlite3. Do logowania klientów oraz przechowywania ich danych logowania został użyty firebase, który ułatwia zarządzanie oraz autentykacje użytkowników.

## Najistotniejsze fragmenty działania klienta

### Fragment kodu odpowiadający za stworzenie oraz obsługę websocketa

```js
const ws = new WebSocket(WS_URL);

ws.onopen = function () {
  console.log("connected");
  ws.send(
    JSON.stringify({
      operation: "/newUser",
      user_id: user?.uid ?? "",
      username: user?.email,
    })
  );
};

ws.onmessage = function (evt) {
  const msg = JSON.parse(evt.data);
  console.log(msg);
};

ws.onclose = function () {
  console.log("disconnected");
};
```

### Fragment kodu odpowiadający za pozyskiwanie listy wiadomości

```js
const fetchMessages = async (recieverId?: string, userId?: string) => {
  if (!userId || !recieverId) {
    return [];
  }

  try {
    const res = await fetch(REST_URL + "/messages", {
      headers: {
        "Content-type": "application/json",
      },
      method: "POST",
      body: JSON.stringify({ sender_id: userId, receiver_id: recieverId }),
    });
    const data: Message[] = await res.json();

    const result = data.sort((msgA, msgB) => {
      const [dateA, dateB] = [
        new Date(msgA.timestamp).getTime(),
        new Date(msgB.timestamp).getTime(),
      ];
      return dateB - dateA;
    });

    return result;
  } catch (err) {
    throw new Error("unexpected error when getting messages");
  }
  return [];
};
```

<br></br>

## Najistotniejsze fragmenty działania serwera

### Fragment kodu odpowiadający za pobranie i zapisanie danych o wiadomości oraz wysłanie wiadomości do odpowiednich uzytkowników

```C++
   else if (strcmp(operation->valuestring, "/message") == 0)
    {
        cJSON *message_json = add_message(json);
        char *friend_id = cJSON_GetObjectItemCaseSensitive(message_json, "to")->valuestring;
        char *to_send = cJSON_Print(message_json);
        ws_sendframe_txt(client, to_send);
        ws_cli_conn_t **receipent = (ws_cli_conn_t **)ht_get(clients, &friend_id, sizeof(int), NULL);
        if (receipent != NULL)
        {
            ws_sendframe_txt(*receipent, to_send);
        }
        free(to_send);
    }
```

### Fragment kodu odpowiadający za wyszukanie wszystkich użytkowników i stworzenie JSONa do przesłania requestem.

```C++
 char *sql = "SELECT user_id, username FROM users";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    cJSON *response_json = cJSON_CreateArray();

    if(stmt!= NULL)
    {
        while(sqlite3_step(stmt) != SQLITE_DONE)
        {
            cJSON *user_json = cJSON_CreateObject();
            cJSON_AddStringToObject(user_json, "user_id", sqlite3_column_text(stmt, 0));
            cJSON_AddStringToObject(user_json, "username", sqlite3_column_text(stmt, 1));
            cJSON_AddItemToArray(response_json, user_json);
        }

    }
    sqlite3_finalize(stmt);

    sqlite3_close(db);
```

### Fragment kodu odpowiedzialny za przechwytywanie requestów i dla wybranego użytkownika wykonanie wybranej operacji.

```C++
 operation operation = get_operation(request);
    http_status status;
    cJSON *user_id = cJSON_GetObjectItemCaseSensitive(request_json, "user_id");

    cJSON *response_json = operation(request_json, user_id, &status);
    char *response = get_response(status, response_json);

    if (response_json != NULL)
    {
        cJSON_Delete(response_json);
    }
```

## Użyte biblioteki po stronie serwera

- **wsSerwer** - mała biblioteka WebSocket napisana w C ułatwiająca korzystanie z serwera. Głównymi elementami tej biblioteki są: łączenie z klientem, rozłączanie z klientem i wysyłanie wiadomości.
- **cJSON** - mały analizator składniowy do plików typu JSON. Pozwala na proste tworzenie plików, odczytywanie i zapisywanie w nich danych.
- **PicoHTTPParser** - mały, szybki analizator składniowy do requestów i odpowiedzi HTTP.
- **SQLite3** - biblioteka do stworzenia prostej bazy danych do przechowywania informacji o użytkownikach i wiadomościach.
- pliki dające możliwość tworzenia Hashtable w C.

## Użyte technologie po stronie klienta

- **Next.js + React** - baza do stworzenia aplikacji webowej
- **tailwind css** - framework ułatwiający stylizowanie komponentów
- **firebase** - serwis udostępniający bazy danych oraz ułatwiający autoryzacje użytkowników
- **react Query** - biblioteka ułatwiająca zarządzanie zapytaniami oraz ich odpowiedziami
