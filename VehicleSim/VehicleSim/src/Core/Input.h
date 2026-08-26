#pragma once 

#include <array>


namespace Input
{
	class Input
	{
	public:
		static Input& Get();

		void OnKeyDown(unsigned char key);
		void OnKeyUp(unsigned char key);
		void OnSpecialDown(int key);
		void OnSpecialUp(int key);

		/// <summary>
		/// True for every frame the key is held
		/// </summary>
		/// <param name="key"></param>
		/// <returns></returns>
		bool IsKeyHeld(unsigned char key);

		/// <summary>
		/// True only on the frame the key went down
		/// </summary>
		/// <param name="key"></param>
		/// <returns></returns>
		bool WasKeyPressed(unsigned char key);

		bool IsSpecialHeld(int key) const;

		/// <summary>
		/// Clear the one shot keys
		/// </summary>
		void ClearFrame();

	private:
		Input() = default;

		static constexpr int KEY_COUNT = 256;
		static constexpr int SPECIAL_COUNT = 256;

		std::array<bool, KEY_COUNT> _held {};
		std::array<bool, KEY_COUNT> _pressedThisFrame {};
		std::array<bool, SPECIAL_COUNT> _specialHeld{};

		/// <summary>
		/// Treat maj key as the same key
		/// </summary>
		/// <param name="key"></param>
		/// <returns></returns>
		static unsigned char Normalize(unsigned char key);
	};
}