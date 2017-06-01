#ifndef PHANTOMPI_BUTTON_HPP
#define PHANTOMPI_BUTTON_HPP 1

namespace phantompi
{
    template <class INPUT_GPIO>
    class PullDownButton
    {
    public:
        PullDownButton(INPUT_GPIO const &input);

        ButtonState getState() const;

    private:
        INPUT_GPIO const * _inputGpio;
    };

    template <typename INPUT_GPIO>
    auto makePullDownButton(INPUT_GPIO const &input)
    {
        return PullDownButton<INPUT_GPIO>(input);
    }

    template <class INPUT_GPIO>
    inline PullDownButton<INPUT_GPIO>::PullDownButton(INPUT_GPIO const &input)
      : _inputGpio{&input} { }

    template <class INPUT_GPIO>
    inline ButtonState PullDownButton<INPUT_GPIO>::getState() const
    {
        static ButtonState const states[] = {
            ButtonState::pressed,
            ButtonState::not_pressed
        };
        auto state = (*_inputGpio)->getState();
        return states[state == GpioState::high];
    }
}

#endif
