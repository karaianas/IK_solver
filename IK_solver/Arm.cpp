#include "Arm.h"
#include <iostream>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#define PI 3.141592


Arm::Arm(vector<Joint*>* js)
{
	for (auto j : *js)
		joints.push_back(j);
	
	initialization();
}

Arm::Arm(int num)
{
	glm::mat4* base = new glm::mat4();
	Joint* R = new Joint(base, 0.0f, 0, 0, 0);
	joints.push_back(R);

	for (int i = 0; i < num - 1; i++)
	{
		Joint* J = new Joint(&joints.back()->W, 1.0f, 0, 0, 0);
		joints.push_back(J);
	}

	initialization();
}

Arm::~Arm()
{
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO1);
}

void Arm::initialization()
{
	this->toWorld = glm::mat4(1.0f);
	angle = 0.0f;

	// Create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO1);
	//glGenBuffers(1, &EBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	//------------------------
	draw_cone();
	draw_shadow();
	//------------------------

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); 

	// Normal Binding
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

	glBindVertexArray(0); // Unbind VAO
}

void Arm::update_buffer(glm::vec3 gp)
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO1);

	vertices.clear();
	indices.clear();
	normals.clear();

	// Create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO1);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	//------------------------
	draw_goal(gp, 0.1f, 5, 5);
	draw_cone();
	draw_shadow();
	//------------------------
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// Normal Binding
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0); // Unbind VAO
}

void Arm::draw_goal(glm::vec3 gp, float radius, int stacks, int slices)
{

	float fstacks = (float)stacks;
	float fslices = (float)slices;
	for (int i = 0; i < slices; i++)
	{
		for (int j = 0; j < stacks; j++)
		{
			// Top left
			vertices.push_back(gp + glm::vec3(
				radius * -cos(2.0f * PI * i / fstacks) * sin(PI * (j + 1.0f) / fslices),
				radius * -cos(PI * (j + 1.0f) / fslices),
				radius * sin(2.0f * PI * i / fstacks) * sin(PI * (j + 1.0f) / fslices)));
			normals.push_back(glm::normalize(glm::vec3(
				-cos(2.0f * PI * i / fstacks) * sin(PI * (j + 1.0f) / fslices),
				-cos(PI * (j + 1.0f) / fslices),
				sin(2.0f * PI * i / fstacks) * sin(PI * (j + 1.0f) / fslices))));
			// Top right
			vertices.push_back(gp + glm::vec3(
				radius * -cos(2.0f * PI * (i + 1.0) / fstacks) * sin(PI * (j + 1.0) / fslices),
				radius * -cos(PI * (j + 1.0) / fslices),
				radius * sin(2.0f * PI * (i + 1.0) / fstacks) * sin(PI * (j + 1.0) / fslices)));
			normals.push_back(glm::normalize(glm::vec3(
				-cos(2.0f * PI * (i + 1.0) / fstacks) * sin(PI * (j + 1.0) / fslices),
				-cos(PI * (j + 1.0) / fslices),
				sin(2.0f * PI * (i + 1.0) / fstacks) * sin(PI * (j + 1.0) / fslices))));
			// Bottom right
			vertices.push_back(gp + glm::vec3(
				radius * -cos(2.0f * PI * (i + 1.0) / fstacks) * sin(PI * j / fslices),
				radius * -cos(PI * j / fslices),
				radius * sin(2.0f * PI * (i + 1.0) / fstacks) * sin(PI * j / fslices)));
			normals.push_back(glm::normalize(glm::vec3(
				-cos(2.0f * PI * (i + 1.0) / fstacks) * sin(PI * j / fslices),
				-cos(PI * j / fslices),
				sin(2.0f * PI * (i + 1.0) / fstacks) * sin(PI * j / fslices))));

			// Need to repeat 2 of the vertices since we can only draw triangles. Eliminates the confusion
			// of array indices.
			// Top left
			vertices.push_back(gp + glm::vec3(
				radius * -cos(2.0f * PI * i / fstacks) * sin(PI * (j + 1.0f) / fslices),
				radius * -cos(PI * (j + 1.0f) / fslices),
				radius * sin(2.0f * PI * i / fstacks) * sin(PI * (j + 1.0f) / fslices)));
			normals.push_back(glm::normalize(glm::vec3(
				-cos(2.0f * PI * i / fstacks) * sin(PI * (j + 1.0f) / fslices),
				-cos(PI * (j + 1.0f) / fslices),
				sin(2.0f * PI * i / fstacks) * sin(PI * (j + 1.0f) / fslices))));
			// Bottom right
			vertices.push_back(gp + glm::vec3(
				radius * -cos(2.0f * PI * (i + 1.0) / fstacks) * sin(PI * j / fslices),
				radius * -cos(PI * j / fslices),
				radius * sin(2.0f * PI * (i + 1.0) / fstacks) * sin(PI * j / fslices)));
			normals.push_back(glm::normalize(glm::vec3(
				-cos(2.0f * PI * (i + 1.0) / fstacks) * sin(PI * j / fslices),
				-cos(PI * j / fslices),
				sin(2.0f * PI * (i + 1.0) / fstacks) * sin(PI * j / fslices))));
			// Bottom left
			vertices.push_back(gp + glm::vec3(
				radius * -cos(2.0f * PI * i / fstacks) * sin(PI * j / fslices),
				radius * -cos(PI * j / fslices),
				radius * sin(2.0f * PI * i / fstacks) * sin(PI * j / fslices)));
			normals.push_back(glm::normalize(glm::vec3(
				-cos(2.0f * PI * i / fstacks) * sin(PI * j / fslices),
				-cos(PI * j / fslices),
				sin(2.0f * PI * i / fstacks) * sin(PI * j / fslices))));
		}
	}
}

void Arm::draw_cone()
{
	for (int i = 0; i < joints.size() - 1; i++)
	{
		Joint* x = joints[i];
		Joint* y = joints[i + 1];
		int theta = 60;
		float r = 0.25f;
		float l = glm::length(x->get_joint_pos() - y->get_joint_pos());
		l /= 4.0f;

		for (int k = 0; k < 4; k++)
		{
			float R0 = r * (4 - k) / 4.0f;
			float R1 = r * (4 - k - 1) / 4.0f;

			for (int j = 0; j < 360 / theta; j++)
			{
				//vertices.push_back(x->get_joint_pos());
				glm::vec3 a0(R0 * cos(j * theta * PI / 180), R0 * sin(j * theta* PI / 180), l * float(k));
				glm::vec3 a1(R0 * cos((j + 1) * theta* PI / 180), R0 * sin((j + 1) * theta* PI / 180), l * float(k));

				glm::vec4 a0_ = x->W * glm::vec4(a0, 1);
				glm::vec4 a1_ = x->W * glm::vec4(a1, 1);

				//float R = r - 0.2f;
				glm::vec3 b0(R1 * cos(j * theta * PI / 180), R1 * sin(j * theta* PI / 180), l * float(k) + l);
				glm::vec3 b1(R1 * cos((j + 1) * theta* PI / 180), R1 * sin((j + 1) * theta* PI / 180), l * float(k) + l);

				glm::vec4 b0_ = x->W * glm::vec4(b0, 1);
				glm::vec4 b1_ = x->W * glm::vec4(b1, 1);

				glm::vec3 A0 = glm::vec3(a0_);
				glm::vec3 A1 = glm::vec3(a1_);
				glm::vec3 B0 = glm::vec3(b0_);
				glm::vec3 B1 = glm::vec3(b1_);

				vertices.push_back(A0);
				vertices.push_back(A1);
				vertices.push_back(B0);

				vertices.push_back(A1);
				vertices.push_back(B1);
				vertices.push_back(B0);

				normals.push_back(glm::cross(A1 - A0, B0 - A0));
				normals.push_back(glm::cross(B0 - A1, A0 - A1));
				normals.push_back(glm::cross(A0 - B0, A1 - B0));

				normals.push_back(glm::cross(B0 - A1, A0 - A1));
				normals.push_back(glm::cross(A1 - A0, B0 - A0));
				normals.push_back(glm::cross(A0 - B0, A1 - B0));

			}
		}
	}
	
}

void Arm::draw_shadow()
{
	int vnum = vertices.size();

	for (int i = 0; i < vnum; i++)
	{
		glm::vec3 ans = vertices[i];
		vertices.push_back(glm::vec3(ans[0], -5.2f, ans[2]));
		normals.push_back(glm::vec3(0, -1, 0));
	}
}

glm::vec2 Arm::singleCCD(int ind, glm::vec3 goal, float alpha, float prev, bool JT)
{
	Joint* j = joints[ind];
	glm::vec3 eff = joints.back()->get_joint_pos();
	glm::vec3 dP = goal - eff;
	glm::vec3 total_angle(0, 0, 0);

	glm::vec3 gj(goal - j->get_joint_pos());
	glm::vec3 ej(eff - j->get_joint_pos());
	float theta = glm::acos(glm::dot(gj, ej) / (glm::length(gj) * glm::length(ej)));
	//cout << ind << ": " << theta << endl;

	if (theta > 0.01f)
	{
		// Get each Jacobian mtx
		glm::mat3 jac(glm::cross(j->get_x_axis(), eff - j->get_joint_pos()), \
			glm::cross(j->get_y_axis(), eff - j->get_joint_pos()), \
			glm::cross(j->get_z_axis(), eff - j->get_joint_pos()));

		// Multiply by dP
		glm::vec3 dAngle = dP * jac;

		// Apply angle change by a small amount
		float thres = alpha * PI / 180.0f;

		float beta = thres / max(thres, max(max(abs(dAngle[0]), abs(dAngle[1])), abs(dAngle[2])));

		glm::vec3 A = dAngle * beta;
		total_angle = A;

		j->set_W(total_angle);

		for (int k = ind + 1; k < joints.size(); k++)
			joints[k]->set_W(glm::vec3(0, 0, 0));

		return glm::vec2(0, theta);
	}
	else
		return glm::vec2(1, theta);
}

glm::vec3 Arm::iterationCCD(int ind, glm::vec3 goal, float alpha, float prev, bool JT)
{
	glm::vec3 state(0, 0, 0);

	glm::vec2 single = singleCCD(ind, goal, alpha, prev, JT);
	
	// That joint state
	if (single[0])
		state[0] = ind - 1;
	else
		state[0] = ind;

	glm::vec3 eff = joints.back()->get_joint_pos();
	glm::vec3 dP = goal - eff;
	float E = glm::length(dP);
	if (E < 0.1f)
		state[1] = 1;
	else
		state[1] = 0;

	state[2] = E;

	return state;
}

glm::vec2 Arm::iteration(glm::vec3 goal, float alpha, bool JT)
{
	glm::vec3 eff = joints.back()->get_joint_pos();
	glm::vec3 dP = goal - eff;
	glm::vec3 total_angle(0, 0, 0);
	glm::vec3 total_angle2(0, 0, 0);

	int count = 0;
	for (auto j : joints)
	{
		// Get each Jacobian mtx
		glm::mat3 jac(glm::cross(j->get_x_axis(), eff - j->get_joint_pos()), \
			glm::cross(j->get_y_axis(), eff - j->get_joint_pos()), \
			glm::cross(j->get_z_axis(), eff - j->get_joint_pos()));

		glm::vec3 dAngle;

		if (!JT)
			dAngle = dP * glm::inverse(glm::transpose(jac) * jac) * glm::transpose(jac);
		else
			dAngle = dP * jac;
		

		// Apply angle change by a small amount
		float thres = alpha * PI / 180.0f;

		float beta = thres / max(thres, max(max(abs(dAngle[0]), abs(dAngle[1])), abs(dAngle[2])));
		//cout << "Joint: " << beta << endl;

		glm::vec3 A = dAngle * beta;
		j->jAngle = A;
	}

	// Update all joint angles
	for (auto j : joints)
	{
		//glm::vec3 ans = j->get_relAngle();
		j->set_W(j->jAngle);
	}

	// Update y-axis rotation
	float y = joints[0]->jAngle[1];
	//for (auto j : joints)
		//j->final(y);

	//glm::vec3 pos = joints.back()->get_joint_pos();
	//fprintf(stderr, "Effector pos(%4.2f, %4.2f, %4.2f)\n", pos[0], pos[1], pos[2]);
	float E = glm::length(goal - joints.back()->get_joint_pos());
	if (E < 0.1f)
	{
		cout << "Goal reached" << endl;
		glm::vec3 pos = joints.back()->get_joint_pos();
		fprintf(stderr, "Effector pos(%4.2f, %4.2f, %4.2f)\n", pos[0], pos[1], pos[2]);
		return glm::vec2(1, E);
	}
	else
		return glm::vec2(0, E);
}

void Arm::iteration(glm::vec3 goal, int num, float threshold, float alpha)
{
	for (int i = 0; i < num; i++)
	{
		iteration(goal, alpha, 1);
		if (glm::length(joints.back()->get_joint_pos() - goal) < threshold)
		{
			cout << "At " << i << "th iteration, the goal has been reached" << endl;
			break;
		}
	}
}

void Arm::draw(GLuint shaderProgram)
{

	// Calculate combination of the model (toWorld), view (camera inverse), and perspective matrices
	glm::mat4 MVP = Window::P * Window::V * toWorld;

	GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	//---------------------------
	glm::mat4 modelview = Window::V * toWorld;
	glm::mat4 model = toWorld;

	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelView = glGetUniformLocation(shaderProgram, "modelview");
	uModel = glGetUniformLocation(shaderProgram, "model");

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelView, 1, GL_FALSE, &modelview[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &model[0][0]);
	//---------------------------

	glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glBindVertexArray(0);
}

void Arm::update()
{
	spin(1.0f);
}

void Arm::spin(float deg)
{
	this->angle += deg;
	if (this->angle > 360.0f || this->angle < -360.0f) this->angle = 0.0f;
	// This creates the matrix to rotate the cube. You'd probably want to change this with the more favorable way of performing a spin
	// which you did in the previous project.
	this->toWorld = glm::rotate(glm::mat4(1.0f), this->angle / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
	//cout << "WTH" << endl;
}