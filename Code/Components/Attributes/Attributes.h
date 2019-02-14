#pragma once
#include <vector>
#include <functional>

//CAttribute
//callback pattern from 
//https://gist.github.com/4poc/3155832

class CAttribute
{
public:
	CAttribute(const float maxValue, const float regenPerS = 0, const float regenOffset = 1)
		: _currentValue(maxValue), _maxValue(maxValue), _regenerationPerSecond(regenPerS),
		_regenerationTimeout(regenOffset), _timer(0)
	{

	}
	typedef const std::function<void()> handler_t;
	enum EAttributeType
	{
		NONE,
		Health,
		Stamina,
		Mana,
		LAST
	};

	bool IsFull() { return _currentValue == _maxValue; }
	bool IsZero() { return _currentValue == ZERO; }

	float GetValue() { return _currentValue; }
	float GetMaxValue() { return _maxValue; }

	void SetValue(const float setTo, const bool pauseRegen = true)
	{
		_currentValue = setTo;
		if (pauseRegen) {
			_timer = 0;
			_canRegen = !pauseRegen;
		}
	}
	void ChangeValue(const float changeBy, const bool pauseRegen = true)
	{
		SetValue(GetValue() + changeBy, pauseRegen);
	}

	void SetRegenerationPerSecond(const float setTo) { _regenerationPerSecond = setTo; }
	void SetRegenerationTimeout(const float setTo) { _regenerationTimeout = setTo; }
	void SetAttributeEmptyCallback(handler_t& h) { handlerList.push_back(&h); }

	void Update(const float fFrameTime)
	{
		if (_regenerationPerSecond <= 0)
			return;

		_timer += fFrameTime;

		if (_canRegen && !IsFull())
			_currentValue = min(_maxValue, _currentValue + _regenerationPerSecond * fFrameTime);

		if (!_canRegen && _timer >= _regenerationTimeout) {
			_canRegen = true;
			_timer = 0;
		}
	}

private:
	void NotifyAllListeners()
	{
		for (auto& handler : handlerList) {
			(*handler)();
			(*handler)();
		}
	}

	float _currentValue;
	float _maxValue;
	float _regenerationPerSecond;
	float _regenerationTimeout;

	float _timer;
	bool _canRegen = false;

	std::vector<handler_t*> handlerList;
};
