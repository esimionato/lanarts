/*
 * GameChat.h
 *
 *  Created on: May 27, 2012
 *      Author: 100397561
 */

#ifndef GAMECHAT_H_
#define GAMECHAT_H_

#include <string>
#include <vector>
#include "../util/game_basic_structs.h"

struct GameState;

/*Represents a coloured message in chat*/
struct ChatMessage {
	std::string sender, message;
	Colour sender_colour, message_colour;
	int exact_copies;
	ChatMessage(const std::string& sender, const std::string& message,
			const Colour& sender_colour = Colour(255, 255, 255),
			const Colour& message_colour = Colour(255, 255, 255)) :
			sender(sender), message(message), sender_colour(sender_colour), message_colour(
					message_colour), exact_copies(1) {
	}
	bool operator==(const ChatMessage& cm) const {
		/*Used to determine if message should be added, or 'exact_copies' incremented*/
		return sender == cm.sender && message == cm.message
				&& sender_colour == cm.sender_colour
				&& message_colour == cm.message_colour;
	}
};

class GameChat {
public:
	void step(GameState* gs);
	void draw(GameState* gs);

	void add_message(const ChatMessage& cm);
	void add_message(const std::string& msg,
			const Colour& colour = Colour(255, 255, 255));
	GameChat();
private:

	void draw_player_chat(GameState* gs);

	std::vector<ChatMessage> messages;
	bool show_chat;
	float fade_out, fade_out_rate;
};

#endif /* GAMECHAT_H_ */
