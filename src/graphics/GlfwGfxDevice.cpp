#include "common_macros.h"
#include "GlfwGfxDevice.h"
#include "ITextureLoader.h"

namespace ncine {

///////////////////////////////////////////////////////////
// STATIC DEFINITIONS
///////////////////////////////////////////////////////////

GLFWwindow *GlfwGfxDevice::windowHandle_ = nullptr;

///////////////////////////////////////////////////////////
// CONSTRUCTORS and DESTRUCTOR
///////////////////////////////////////////////////////////

GlfwGfxDevice::GlfwGfxDevice(int width, int height, const GLContextInfo &glContextInfo, const DisplayMode &mode, bool isFullScreen)
	: IGfxDevice(width, height, glContextInfo, mode, isFullScreen)
{
	initGraphics();
	initDevice();
}

GlfwGfxDevice::~GlfwGfxDevice()
{
	glfwDestroyWindow(windowHandle_);
	windowHandle_ = nullptr;
	glfwTerminate();
}

///////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////

void GlfwGfxDevice::setResolution(int width, int height)
{
	// change resolution only in the case it really changes
	if (width != width_ || height != height_)
	{
		width_ = width;
		height_ = height;

		glfwDestroyWindow(windowHandle_);
		windowHandle_ = nullptr;
		initDevice();
	}
}

void GlfwGfxDevice::toggleFullScreen()
{
	isFullScreen_ = !isFullScreen_;

	glfwDestroyWindow(windowHandle_);
	windowHandle_ = nullptr;
	initDevice();
}

void GlfwGfxDevice::setWindowIcon(const char *windowIconFilename)
{
	nctl::UniquePtr<ITextureLoader> image = ITextureLoader::createFromFile(windowIconFilename);
	GLFWimage glfwImage;
	glfwImage.width = image->width();
	glfwImage.height = image->height();
	glfwImage.pixels = const_cast<unsigned char *>(image->pixels());

	glfwSetWindowIcon(windowHandle_, 1, &glfwImage);
}

///////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
///////////////////////////////////////////////////////////

void GlfwGfxDevice::initGraphics()
{
	glfwSetErrorCallback(errorCallback);
	FATAL_ASSERT_MSG(glfwInit() == GL_TRUE, "glfwInit() failed");
}

void GlfwGfxDevice::initDevice()
{
	// asking for a video mode that does not change current screen resolution
	if (width_ == 0 || height_ == 0)
	{
		const GLFWvidmode *vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		width_ = vidMode->width;
		height_ = vidMode->height;
	}

	GLFWmonitor *monitor = nullptr;
	if (isFullScreen_)
		monitor = glfwGetPrimaryMonitor();

	// setting window hints and creating a window with GLFW
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, static_cast<int>(glContextInfo_.majorVersion));
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, static_cast<int>(glContextInfo_.minorVersion));
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, glContextInfo_.debugContext ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_RED_BITS, static_cast<int>(mode_.redBits()));
	glfwWindowHint(GLFW_GREEN_BITS, static_cast<int>(mode_.greenBits()));
	glfwWindowHint(GLFW_BLUE_BITS, static_cast<int>(mode_.blueBits()));
	glfwWindowHint(GLFW_ALPHA_BITS, static_cast<int>(mode_.alphaBits()));
	glfwWindowHint(GLFW_DEPTH_BITS, static_cast<int>(mode_.depthBits()));
	glfwWindowHint(GLFW_STENCIL_BITS, static_cast<int>(mode_.stencilBits()));
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, glContextInfo_.forwardCompatible ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, glContextInfo_.coreProfile ? GLFW_OPENGL_CORE_PROFILE : GLFW_OPENGL_COMPAT_PROFILE);

	windowHandle_ = glfwCreateWindow(width_, height_, "", monitor, nullptr);
	FATAL_ASSERT_MSG(windowHandle_, "glfwCreateWindow() failed");

	glfwMakeContextCurrent(windowHandle_);

	const int interval = mode_.hasVSync() ? 1 : 0;
	glfwSwapInterval(interval);

#ifdef WITH_GLEW
	const GLenum err = glewInit();
	FATAL_ASSERT_MSG_X(err == GLEW_OK, "GLEW error: %s", glewGetErrorString(err));

	glContextInfo_.debugContext = glContextInfo_.debugContext && glewIsSupported("GL_ARB_debug_output");
#endif
}

void GlfwGfxDevice::errorCallback(int error, const char *description)
{
	LOGE_X("GLFW error %d: \"%s\"", error, description);
}

}
