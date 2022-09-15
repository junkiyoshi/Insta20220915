#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(255);
	ofSetLineWidth(1.2);
	ofEnableDepthTest();

	this->mesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
	this->font.loadFont("fonts/Kazesawa-Bold.ttf", 120, true, true, true);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	vector<float> min_distance_list;
	this->mesh.clear();

	for (int i = 0; i < 1800; i++) {

		auto vertex = glm::vec3(
			ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.001), 0, 1, -200, 200),
			ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.001), 0, 1, -200, 200),
			ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.001), 0, 1, -100, 100));
		this->mesh.addVertex(vertex);
		min_distance_list.push_back(20);
	}

	string word = "DAY";
	auto path_list = font.getStringAsPoints(word, true, false);

	for (auto path : path_list) {

		auto outline = path.getOutline();
		
		for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

			outline[outline_index] = outline[outline_index].getResampledBySpacing(15);
			auto vertices = outline[outline_index].getVertices();

			for (int vertices_index = 0; vertices_index < vertices.size(); vertices_index++) {

				auto location = glm::vec3(vertices[vertices_index].x - this->font.stringWidth(word) * 0.5, vertices[vertices_index].y + this->font.stringHeight(word) * 0.5, 0);
				this->mesh.addVertex(location);
			}
		
		}
	}

	ofColor color;
	for (int i = 0; i < this->mesh.getNumVertices(); i++) {

		for (int k = 0; k < this->mesh.getNumVertices(); k++) {

			if (i == k) { continue; }

			auto distance = glm::distance(this->mesh.getVertex(i), this->mesh.getVertex(k));
			if (distance < 20) {

				this->mesh.addIndex(i);
				this->mesh.addIndex(k);

				if (distance < min_distance_list[i]) {

					min_distance_list[i] = distance;
				}
			}
		}

		color.setHsb(ofRandom(255), 200, 255);
		this->mesh.addColor(ofColor(color, ofMap(min_distance_list[i], 0, 20, 255, 0)));
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(180);
	ofRotateY(45 * sin(ofGetFrameNum() * 0.035));

	this->mesh.draw();

	for (int i = 0; i < this->mesh.getNumVertices(); i++) {

		ofSetColor(this->mesh.getColor(i));
		ofDrawSphere(this->mesh.getVertex(i), 1.5);
	}

	this->cam.end();
}

//--------------------------------------------------------------
glm::vec3 ofApp::make_point(float R, float r, float u, float v) {

	// 数学デッサン教室 描いて楽しむ数学たち　P.31

	u *= DEG_TO_RAD;
	v *= DEG_TO_RAD;

	auto x = (R + r * cos(u)) * cos(v);
	auto y = (R + r * cos(u)) * sin(v);
	auto z = r * sin(u);

	return glm::vec3(x, y, z);
}


//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}