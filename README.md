To run the project in Visual Studio 2022, follow these steps:

1.copy url
![image](https://github.com/user-attachments/assets/0ce48711-a0ad-4ae0-af29-f7ecd43c26fe)

2. Open Visual Studio 2022 and click on "Clone a repository", then enter the copied URL.

3. Right-click on the solution, select "Set Startup Projects", and change it to "Current Selection".

4.Select cpp code within project(main.cpp)
![image](https://github.com/user-attachments/assets/08f067fb-3c27-4d48-9b56-4b2903c9883d)

5.Press Ctrl + F5 to compile, debug, and build the project.
------------------------------------------------------------------------------------------------------
*result Screenshot*
Q1.Immediate Mode
main.cpp result image
![image](https://github.com/user-attachments/assets/b02beaa6-0f2f-4fca-8cc3-97fac3bd5d04)

Q2.Vertex Array
main.cpp result image
![image](https://github.com/user-attachments/assets/0d948f4b-14ee-4549-8868-c2b7e6a0782b)

------------------------------------------------------------------------------------------------------
*Code Explanation*
Q1.Immediate Mode
Shading Parameter
![image](https://github.com/user-attachments/assets/801a9044-5baf-4e74-bed4-f2cf51447224)

viewport transformation
int Width = 1280, Height = 1280;

transform
![image](https://github.com/user-attachments/assets/493c3b54-a7f7-416b-81a5-e5f177327199)

calculate fps while in roop
![image](https://github.com/user-attachments/assets/886d4aa0-6ec7-45f2-a1b8-94bbae177953)

immedeiate mode
![image](https://github.com/user-attachments/assets/0cf2a916-34ad-4cd4-83fd-7b34c47e8e7b)
send glvertex3f and glnormal3f every frame

disable back face culling
![image](https://github.com/user-attachments/assets/bba2cf4f-9b21-4f55-936d-af0bfcb0309d)

--------------------------------------------------------------------------------------------------------
Q2.Vertex Array
deferent pipeline  and other optins is smae as Q1 immedeiate mode
set up vertex struct
![image](https://github.com/user-attachments/assets/ea07db09-da5a-489c-9db3-3e3be3fb731d)

set up VAO
![image](https://github.com/user-attachments/assets/596a410a-302f-4b8f-a7ce-00ba15e72caf)

binding VAO , VBO, EBO
register vertex pointer
upload to Gpu memory 

rendering
![image](https://github.com/user-attachments/assets/68882948-5fb8-4f3b-a586-cf8eb3d2a1bf)
