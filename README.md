# ClosedLocker (Phiên bản 2026.17.06)

Đây là một bản fork (nhánh phát triển cải tiến) từ dự án bộ gõ tiếng Việt nổi tiếng **OpenKey v2.0.5** dành cho Windows.

* **Dự án gốc:** [OpenKey](http://open-key.org)
* **Mã nguồn bản gốc:** [https://github.com/tuyenvm/OpenKey](https://github.com/tuyenvm/OpenKey)

---

## Lời cảm ơn
Xin chân thành cảm ơn tác giả gốc **Mai Vũ Tuyên** vì đã phát triển bộ gõ tiếng Việt nguồn mở đa nền tảng tuyệt vời này và chia sẻ mã nguồn rộng rãi dưới giấy phép GPL. Nhờ nền tảng OpenKey vững chắc, phiên bản cải tiến này đã được hoàn thiện.

---

## Các cải tiến & Thay đổi so với bản gốc 2.0.5

Bản fork này được phát triển dưới tên gọi mới là **ClosedLocker** (bản phát hành **2026.17.06**) bởi tác giả **Danniskill**, tập trung khắc phục triệt để các lỗi vận hành phổ biến trên Windows và tinh gọn giao diện điều khiển:

### 1. Sửa lỗi kẹt Shift ảo (Lỗi viết hoa nguyên âm: `"tÔi khÔng rẢnh"`)
* **Mô tả lỗi:** Khi phần mềm sử dụng phương pháp dán thông qua tổ hợp phím ảo `Shift + Insert` để thay thế ký tự có dấu, hệ điều hành đôi khi bỏ lỡ hoặc giữ lại sự kiện nhấn giữ phím Shift ảo (nhất là khi người dùng thao tác nhanh hoặc chuyển đổi cửa sổ). Trạng thái Shift ảo bị kẹt khiến các nguyên âm tiếp theo bị viết hoa không mong muốn.
* **Cách khắc phục:** Mã nguồn đã bổ sung cơ chế kiểm tra cờ `LLKHF_INJECTED` để bỏ qua các phím ảo do chính bộ gõ giả lập, đồng thời tự động đồng bộ lại trạng thái thực tế của các phím bổ trợ (Shift/Ctrl/Alt) thông qua API `GetKeyState` ngay khi người dùng nhấn bất kỳ phím thường nào khác.

### 2. Sửa lỗi mất Hook bàn phím (Tự động ngừng gõ tiếng Việt)
* **Mô tả lỗi:** Windows đôi khi tự động gỡ bỏ các Low-Level Keyboard Hooks nếu callback phản hồi chậm hoặc bị xung đột bởi các phần mềm khác.
* **Cách khắc phục:** Bổ sung cơ chế tự động cài đặt lại (reinstall) hook bàn phím và chuột mỗi khi người dùng thay đổi cửa sổ làm việc tích cực (Foreground Window), đảm bảo bộ gõ luôn hoạt động ổn định liên tục.

### 3. Thay đổi giao diện & Tinh giản
* **Loại bỏ tab "Thông tin" (About tab):** Rút gọn giao diện chính chỉ giữ lại 3 tab điều khiển cốt lõi: **Bộ gõ**, **Gõ tắt**, và **Hệ thống** để tạo sự tối giản và tối ưu trải nghiệm.
* **Đổi tên thương hiệu (Rebranding):** Đổi tên toàn bộ ứng dụng thành **ClosedLocker**, cập nhật tệp tin cập nhật đi kèm thành `ClosedLockerUpdate.exe`.
* **Căn chỉnh UI:** Sửa lỗi tràn chữ nút bấm ở chân giao diện (`Tắt ClosedLocker`) bằng cách nâng chiều rộng các nút bấm từ 71 lên 81 dialog units.
* **Xử lý mã hóa giao diện:** Sử dụng cờ biên dịch `/utf-8` để hiển thị tiếng Việt có dấu chuẩn xác 100% trên các tab, tránh hiện tượng lỗi phông (Mojibake).

---

## Thông tin Bản phát hành
* **Phiên bản:** `2026.17.06`
* **Tác giả:** `Danniskill © 2026`
* **Nền tảng hỗ trợ:** Windows (x64 & Win32)
* **Trình biên dịch:** MSVC v145 (Visual Studio 2022 v18)

---

## Giấy phép
Dự án tiếp tục được phát hành dưới giấy phép mã nguồn mở GPL của bản gốc.
