#ifndef PHANTOMPI_BUTTON_HPP
#define PHANTOMPI_BUTTON_HPP 1

namespace phantompi
{
    template <typename INPUT_GPIO, GpioState PUSHED_VALUE, GpioState UNPUSHED_VALUE>
    class Button
    {
    public:
        Button(INPUT_GPIO const &input);

        ButtonState getState() const;

    private:
        INPUT_GPIO const * _inputGpio;
    };

    template <typename INPUT_GPIO>
    using PullDownButton = Button<INPUT_GPIO, GpioState::low, GpioState::high>;

    template <typename INPUT_GPIO>
    using PullUpButton = Button<INPUT_GPIO, GpioState::high, GpioState::low>;

    template <typename INPUT_GPIO>
    auto makePullDownButton(INPUT_GPIO const &input)
    {
        return PullDownButton<INPUT_GPIO>(input);
    }

    template <typename INPUT_GPIO>
    auto makePullUpButton(INPUT_GPIO const &input)
    {
        return PullUpButton<INPUT_GPIO>(input);
    }

    template <typename INPUT_GPIO, GpioState PUSHED_VALUE, GpioState UNPUSHED_VALUE>
    inline Button<INPUT_GPIO, PUSHED_VALUE, UNPUSHED_VALUE>::Button(INPUT_GPIO const &input)
      : _inputGpio{&input} { }

    template <typename INPUT_GPIO, GpioState PUSHED_VALUE, GpioState UNPUSHED_VALUE>
    inline ButtonState Button<INPUT_GPIO, PUSHED_VALUE, UNPUSHED_VALUE>::getState() const
    {
        static ButtonState const states[] = {
            ButtonState::not_pressed,
            ButtonState::pressed
        };
        auto state = (*_inputGpio)->getState();
        return states[state == PUSHED_VALUE];
    }
}

#endif
