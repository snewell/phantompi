#ifndef PHANTOMPI_BUTTON_HPP
#define PHANTOMPI_BUTTON_HPP 1

#include <phantompi/types.hpp>

namespace phantompi
{
    /** @brief a physical button whose state can be either
     *         ButtonState::pressed or ButtonState::not_pressed
     *
     * This is a helper class to deal with buttons.  This class is not intended
     * to be used directly; instances can be created using makePullUpButton or
     * makePullDownButton.
     *
     * @tparam INPUT_GPIO
     *      a device that implements the @a INPUT_GPIO concept
     *
     * @tparam PUSHED_VALUE
     *      the GpioState that represents a pushed button
     *
     * @sa makePullUpButton
     * @sa makePullDownButton
     *
     * @ingroup devices
     */
    template <typename INPUT_GPIO, GpioState PUSHED_VALUE>
    class Button
    {
    public:
        /** @brief construct a Button
         *
         * @param input A type representing an @a INPUT_GPIO.  @a input is used
         * to detemrine if the Button is ButtonState::pressed or
         * ButtonState::not_pressed.
         */
        Button(INPUT_GPIO const & input);

        /// @brief determine if this Button is pressed or not
        ButtonState getState() const;

    private:
        INPUT_GPIO const * _inputGpio;
    };

    template <typename INPUT_GPIO>
    using PullDownButton = Button<INPUT_GPIO, GpioState::low>;

    template <typename INPUT_GPIO>
    using PullUpButton = Button<INPUT_GPIO, GpioState::high>;

    /** @brief create a pull down button
     *
     * A pull down button is a physical device that presents a high value while
     * open and a low value while closed.
     *
     * @param input a device that implements the @a INPUT_GPIO concept
     */
    template <typename INPUT_GPIO>
    inline auto makePullDownButton(INPUT_GPIO const & input)
    {
        return PullDownButton<INPUT_GPIO>(input);
    }

    /** @brief create a pull up button
     *
     * A pull down button is a physical device that presents a low value while
     * open and a high value while closed.
     *
     * @param input a device that implements the @a INPUT_GPIO concept
     */
    template <typename INPUT_GPIO>
    inline auto makePullUpButton(INPUT_GPIO const & input)
    {
        return PullUpButton<INPUT_GPIO>(input);
    }

    template <typename INPUT_GPIO, GpioState PUSHED_VALUE>
    inline Button<INPUT_GPIO, PUSHED_VALUE>::Button(INPUT_GPIO const & input)
      : _inputGpio{&input}
    {
    }

    template <typename INPUT_GPIO, GpioState PUSHED_VALUE>
    inline ButtonState Button<INPUT_GPIO, PUSHED_VALUE>::getState() const
    {
        static ButtonState const states[] = {ButtonState::not_pressed,
                                             ButtonState::pressed};
        auto state = (*_inputGpio)->getState();
        return states[state == PUSHED_VALUE];
    }
}

#endif
