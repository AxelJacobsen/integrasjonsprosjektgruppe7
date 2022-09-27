#pragma once

// TODO: place in own namespace and remove 'Component' suffix
namespace comp {
	struct Sprite {
		SpriteInfo info;
	};

	struct Position {
		glm::vec2 pos;
	};

	struct Rotation {
		float angle;
	};

	struct Scale {
		float scale;
	};
}
