#RTII and Memory Management

----

###Smart Pointer

1. boost::scoped_ptr/boost::scoped_array<br/>
不能copy，且只會存活在這個scope，當離開這個scope時會自動free

2. boost::shared_ptr/boost::shared_array<br/>
ownership可轉交，會紀錄現在有幾個人指向他，當沒人指向他時才free<br/>
指像他的人都是用reference，並不是複製！<br/>

3. boost::make_shared<br/>
ex: auto p = boost::make_shared<int>(1) ;<br/>

4. boost::weak_ptr
避免smart pointer循環reference，導致無法自動free，memory leak<br/>
這時如果是用weak_ptr，則不會計算refer count，在生命週期結束時會自動free掉<br/>
可以利用weak_ptr::lock()，這會檢查該refer的ptr有無被free掉，<br/>
引用原文<br/>
invoking lock() returns a shared pointer that points to a valid object if one exists at the time of the call. If not, the shared pointer is set to 0 and is equivalent to a null pointer.<br/>
參考:<br/>
http://www.cnblogs.com/tianfang/archive/2008/09/20/1294590.html

5. boost::intrusive_ptr<br/>
跟shared_ptr很像，解釋有點複雜，直接看<br/>
http://www.cnblogs.com/edwardlost/archive/2011/02/17/1957019.html


----

