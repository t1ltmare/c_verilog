#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <GLFW/glfw3.h>

int main(int argc, char* argv[]){
   glfwInit(); // Самая простая инициализация GLFW3
   auto window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL); // Самый простой вариант создания окна с помощью GLFW#
   glfwMakeContextCurrent(window);// Указываем OpenGL работать с данным окном
   glbinding::initialize(glfwGetProcAddress); // Инициализируем glbinding, с помощью glfwGetProcAddress из GLFW3 (чтобы glbinding "знал" где какие функции OpenGL)


   // С этого момента, можно вызывать функции OpenGL, 
   // при необходимости вызывая #undef для констант OpenGL, 
   // которые могут высыпаться из GLFW3, но которые не 
   // понимает glbinding; 
   // весь OpenGL находится в пространстве имён gl в glbinding,
   // поэтому перед всем, что относится к OpenGL может быть нужно писать gl:: (или можно написать using namespace gl; или можно написать using gl::<то, что хотим из OpenGL)
  

   // Дальше типовая реализация:

   gl::glClearColor(0.5, 0.5, 0.5, 1); // Задаём цвет экрана серый (r, g, b, a)
   while (!glfwWindowShouldClose(window)) {// Пока окно открыто
     glfwPollEvents(); // Проверяем события окна
#undef GL_COLOR_BUFFER_BIT // т.к. может вывалиться из GLFW3 - то о чём писал выше
     gl::glClear(gl::GL_COLOR_BUFFER_BIT); // Очищаем экран

     // Тут прописываем обновление экрана с помощью OpenGL

     glfwSwapBuffers(window); // Обновляем графику, согласно тому, что мы указали OpenGL 
   }
   glfwTerminate(); // Очищаем всякое барахло
}
