#pragma once

template<class Vertex>
class DefaultVertexShader
{
public:
	typedef Vertex Output;
public:
	void BindRotation(const Mat3& rotation_in)
	{
		rotation = rotation_in;
	}
	void BindTranslation(const Vec3& translation_in)
	{
		translation = translation_in;
	}
	Output operator()(const Vertex& in) const
	{
		return{ in.pos * rotation + translation,in };
	}
	const Mat3 get_rotation() const
	{
		return rotation;
	}
private:
	Mat3 rotation;
	Vec3 translation;

};
