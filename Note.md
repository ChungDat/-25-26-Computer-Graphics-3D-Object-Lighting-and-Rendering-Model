* Nói về môi trường do hoa OpenGL, các hàm đồ hoạ từ OpenGL, (vẽ, tô màu, glsl), VBO, VAO
* Nói về các bước shader, (model -> world -> eye -> clip -> screen)
* Quy trình hiển thị đồ hoạ (sách Graphics Shader)
* Nói về framework (cần công đoạn, hàm nào, có sẵn/lập trình) trong chương trình (tạo ánh sáng, tạo vật thể từ model -> world (model được lấy từ đâu: point và normal vector / point cloud từ file .obj...), tính toán normal vector, render trong world, tạo ma trận world -> eye, chuyển tất cả sang eye, eye -> clip -> screen), mục đích của chương trình

* Một pixel màu = màu của đối tượng + mô hình chiếu sáng + khả năng hấp thụ phản xạ ánh sáng
* Biểu thức toán của mô hình chiếu sáng ảnh hưởng tới vật như thế nào
* Mô hình toán phản xạ
* Khả năng hấp thụ ánh sáng có mấy loại? công thức toán
* Ambient ngày đêm, cùng nguồn sáng vẫn cho ra màu khác nhau
* Vì sao Diffuse không cần vector R (đồng nhất, đẳng hướng)