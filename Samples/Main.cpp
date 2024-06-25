#include <Trinity/RHI/RHI.h>
#include <Trinity/Core/Logging/Log.h>
#include <GLFW/glfw3.h>
#include <crtdbg.h>
int main()
{
	{
		glfwInit();
		TLog::AddMessageHandlerCallback(TLog::DefaultStdoutMessageHandler);
		TRHI* Rhi = TRHI::CreateRHI(TRHIType::EVulkan);
		TRHI::DestroyRHI(Rhi);
		TLog::ClearAllMessageHandlerCallbacks();
		glfwTerminate();
	}
	_CrtDumpMemoryLeaks();
	return 0;
}