
template <typename BUTTON>
void checkButton(BUTTON const & button)
{
    static char const * const messages[] = {"not pressed", "pressed"};
    auto const state = button.getState();
    auto const message =
        messages[static_cast<std::underlying_type_t<phantompi::ButtonState>>(
            state)];
    std::printf("Button is %s\n", message);
}
