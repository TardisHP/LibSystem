# LibSystem
created by xyf, zzx and lj
---
### 基本数据对象
|object|class|property|
|---|---|---|
|书|Book|书号, 类别, 书名, 出版社, 年份, 作者, 价格, 剩余库存|
|借书证|Card|卡号, 姓名, 单位, 身份(教师或学生)|
|借书记录|Borrow|卡号, 书号, 借书日期, 还书日期|
### 基本功能模块
- `int storeBook(Book)`: 图书入库模块。向图书库中注册(添加)一本新书。
- `int queryBook(Book[], int)`: 图书查询模块。
- `int removeBook(int)`: 图书删除模块，从图书库中删除一本书。如果还有人尚未归还这本书，那么删除操作将失败。
- `int incBookStock(int, int)`: 图书增加库存模块。为图书库中的某一本书增加库存。其中库存增量deltaStock可正可负，若为负数，则需要保证最终库存是一个非负数。 
- `int modifyBookInfo(Book)`: 图书修改模块。修改已入库图书的基本信息，该接口不能修改图书的书号和存量。
- `int borrowBook(Borrow)`: 借书模块。根据给定的书号、卡号和借书时间添加一条借书记录，然后更新库存。若用户此前已经借过这本书但尚未归还，那么借书操作将失败。
- `int returnBook(Borrow)`: 还书模块。根据给定的书号、卡号和还书时间，查询对应的借书记录，并补充归还时间，然后更新库存。
- `void showBorrowHistory(int)`: 借书记录查询模块。查询某个用户的借书记录，按照借书时间递减、书号递增的方式排序。
- `int registerCard(Card)`: 借书证注册模块。注册一个借书证，若借书证已经存在，则该操作将失败。当且仅当<姓名, 单位, 身份>均相同时，才认为两张借书证相同。
- `int removeCard(int)`: 注销借书证模块。如果该借书证还有未归还的图书，那么删除操作将失败。
- `void showCards()`: 借书证查询模块。列出所有的借书证。
### 返回码
|status|meaning|
|---|---|
|1xx|succeeded|
|2xx|user actions|
|4xx|failed|
