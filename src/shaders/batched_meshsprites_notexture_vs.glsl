uniform mat4 projection;
uniform mat4 view;

struct MeshSpriteInstance
{
	mat4 modelView;
	vec4 color;
	vec2 spriteSize;
};

layout (std140) uniform InstancesBlock
{
#ifdef WITH_FIXED_BATCH_SIZE
	MeshSpriteInstance[BATCH_SIZE] instances;
#else
	MeshSpriteInstance[682] instances;
#endif
} block;

in vec2 aPosition;
in uint aMeshIndex;
out vec4 vColor;

#define i block.instances[aMeshIndex]

void main()
{
	vec4 position = vec4(aPosition.x * i.spriteSize.x, aPosition.y * i.spriteSize.y, 0.0, 1.0);

	gl_Position = projection * view * i.modelView * position;
	vColor = i.color;
}
