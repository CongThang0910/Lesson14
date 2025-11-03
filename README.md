# 🧩 BÀI TẬP STM32 – EVENT GROUPS, DELAY, SLEEP MODE & STANDBY MODE


---

# ✅ BÀI 1 – Chương trình dùng Event Groups điều phối 3 Task

## 🎯 Yêu cầu
- Dùng FreeRTOS Event Groups.
- Có **1 Task chính** điều phối các Task còn lại.
- Có **3 Task phụ**, mỗi Task thực hiện theo bit điều khiển riêng.
- Task chính điều khiển theo hai cách:
  - Kích hoạt lần lượt Task1 → Task2 → Task3.
  - Kích hoạt **đồng thời** cả 3 Task.
- Mỗi Task phụ sáng LED khi được kích hoạt.

## ✅ Cách hoạt động 
- 3 bit sự kiện:
  - `BIT_TASK1`
  - `BIT_TASK2`
  - `BIT_TASK3`
- Task1, Task2, Task3 sẽ:
  1. `xEventGroupWaitBits()` để chờ sự kiện.
  2. Khi có bit → bật LED tương ứng 500ms rồi tắt.
- Task_Control:
  - Gửi từng bit để chạy từng task.
  - Sau đó gửi cả 3 bit để chạy đồng thời.

## ✅ Kiến thức áp dụng
- FreeRTOS Event Groups.
- Cơ chế đồng bộ nhiều Task.
- Chạy đơn lẻ và chạy song song.

---

# ✅ BÀI 2 – So sánh Delay (bận CPU) và Sleep (tiết kiệm điện)

Bài 2 gồm 2 chương trình:  
1️⃣ Dùng Delay SysTick (CPU bận 100%).  
2️⃣ Dùng Delay + Sleep (WFI) để tiết kiệm điện.

---

# ✅ Bài 2.1 – Chương trình Delay bằng SysTick (bận CPU)

## 🎯 Yêu cầu
- Tự viết hàm Delay bằng SysTick.
- LED PA8 chớp mỗi 5 giây.
- CPU chạy liên tục → không tiết kiệm điện.

## ✅ Cách hoạt động 
- SysTick tải giá trị 72000-1 → tạo chu kỳ 1ms.
- Delay dùng vòng `while` bận CPU.
- LED toggle PA8 mỗi 5 giây.

## ✅ Nhận xét
- Đúng yêu cầu bài tập.
- CPU luôn hoạt động → tốn năng lượng.

---

# ✅ Bài 2.2 – Chương trình Delay dùng Sleep (WFI)

## 🎯 Yêu cầu
- 2 chương trình cùng chức năng:
  - Một dùng Delay busy-wait (NOP)
  - Một dùng Delay Sleep (WFI)
- So sánh mức tiêu thụ điện khi chạy.

## ✅ Cách hoạt động 
- SysTick tạo ngắt mỗi 1ms.
- Hàm `delay_ms()`:
  - Nếu `USE_SLEEP = 1` → CPU ngủ (`__WFI()`).
  - Nếu `USE_SLEEP = 0` → CPU chạy NOP liên tục.
- LED PA8 bật/tắt mỗi 5 giây.

## ✅ Nhận xét
- `__WFI()` giảm tiêu thụ điện đáng kể.
- Đúng yêu cầu bài: so sánh Sleep vs Busy-Wait.

---

# ✅ BÀI 3 – Gửi UART → chuyển sang Standby → đo điện năng

## 🎯 Yêu cầu
- STM32 gửi dữ liệu định kỳ qua UART cho máy tính.
- Sau khi gửi xong → chuyển vào **chế độ Standby**.
- Đo dòng tiêu thụ:
  - Khi đang gửi UART.
  - Khi đang ở Standby.


## ✅ Mô tả theo yêu cầu bài 3 (đúng đề bài)

### 1️⃣ STM32 gửi dữ liệu UART định kỳ
- UART được bật clock và cấu hình Baud (ví dụ 9600).
- Lặp lại việc gửi dữ liệu bằng polling hoặc interrupt.

### 2️⃣ Sau một thời gian, MCU chuyển sang Standby
- Xóa cờ WakeUp:
  ```c
  PWR->CR |= PWR_CR_CWUF;
